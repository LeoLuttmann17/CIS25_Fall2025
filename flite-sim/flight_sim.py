"""
flight_sim.py — simple realistic flight simulator (no images)
Requires: pygame
Run: python flight_sim.py
"""

import pygame
import sys
import math
import random
from collections import deque

# ---------------------- Utility functions ----------------------
def clamp(x, a, b):
    return max(a, min(b, x))

def wrap_angle(a):
    # wrap to [-pi, pi]
    while a > math.pi:
        a -= 2*math.pi
    while a < -math.pi:
        a += 2*math.pi
    return a

def rotation_matrix(roll, pitch, yaw):
    # Returns 3x3 rotation matrix from body -> world using ZYX (yaw-pitch-roll)
    cr = math.cos(roll); sr = math.sin(roll)
    cp = math.cos(pitch); sp = math.sin(pitch)
    cy = math.cos(yaw); sy = math.sin(yaw)
    # R = R_z(yaw) * R_y(pitch) * R_x(roll)
    R = [
        [cy*cp, cy*sp*sr - sy*cr, cy*sp*cr + sy*sr],
        [sy*cp, sy*sp*sr + cy*cr, sy*sp*cr - cy*sr],
        [-sp,   cp*sr,            cp*cr]
    ]
    return R

def mat_vec_mul(M, v):
    return [M[0][0]*v[0] + M[0][1]*v[1] + M[0][2]*v[2],
            M[1][0]*v[0] + M[1][1]*v[1] + M[1][2]*v[2],
            M[2][0]*v[0] + M[2][1]*v[1] + M[2][2]*v[2]]

def vec_length(v):
    return math.sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2])

def vec_add(a,b): return [a[0]+b[0], a[1]+b[1], a[2]+b[2]]
def vec_sub(a,b): return [a[0]-b[0], a[1]-b[1], a[2]-b[2]]
def vec_scale(a,s): return [a[0]*s, a[1]*s, a[2]*s]

# ---------------------- Aircraft parameters ----------------------
class Aircraft:
    def __init__(self):
        # Physical parameters
        self.mass = 1200.0            # kg (light GA aircraft)
        self.wing_area = 16.2         # m^2
        self.wing_span = 10.0         # m
        self.chord = self.wing_area / self.wing_span
        self.Ix = 1000.0              # roll moment of inertia
        self.Iy = 1500.0              # pitch
        self.Iz = 2000.0              # yaw

        # Aerodynamic coefficients (simple model)
        self.Cl0 = 0.2                # lift coefficient at alpha=0
        self.Cl_alpha = 5.5           # per radian (slope)
        self.Cd0 = 0.02               # parasitic drag
        self.K = 0.045                # induced drag factor
        self.alpha_stall = math.radians(15.0)  # stall angle (rad)
        self.alpha_cutoff_width = math.radians(5.0)  # stall falloff width

        # Control effectiveness (moments per rad and per dynamic pressure)
        self.elevator_effect = 0.8
        self.aileron_effect = 1.2
        self.rudder_effect = 0.6
        self.thrust_max = 20000.0     # N max thrust (single engine equivalent)
        self.max_speed = 120.0        # m/s (for limits)

        # Aerodynamic center offsets (for moments)
        self.x_cg = 0.0
        self.x_ac = 0.1

        # State vectors
        # position in world coordinates: x (north), y (east), z (down positive)
        self.pos = [0.0, 0.0, -100.0]  # start 100 m above ground (z negative uses upward)
        self.vel = [40.0, 0.0, 0.0]    # body- or world-frame? We'll maintain in world frame
        self.phi = 0.0                 # roll
        self.theta = 0.05              # pitch
        self.psi = 0.0                 # yaw

        self.p = 0.0                   # roll rate (rad/s)
        self.q = 0.0                   # pitch rate
        self.r = 0.0                   # yaw rate

        # Controls (normalized)
        self.throttle = 0.5
        self.elevator = 0.0   # -1..1 (nose down..up)
        self.aileron = 0.0    # -1..1 (left..right)
        self.rudder = 0.0     # -1..1 (left..right)

        # Environment
        self.rho = 1.225      # kg/m^3 (sea level)
        self.gravity = 9.80665

        # Misc
        self.on_ground = False
        self.landed = False
        self.crashed = False

    # Aerodynamic lift coefficient with stall model (smooth fall-off)
    def compute_Cl(self, alpha):
        # Linear part
        Cl_lin = self.Cl0 + self.Cl_alpha * alpha
        # Stall falloff using a tanh smoothing around alpha_stall
        stall_factor = 0.5 * (1.0 - math.tanh((abs(alpha) - self.alpha_stall) / (self.alpha_cutoff_width)))
        return Cl_lin * stall_factor

    def compute_Cd(self, alpha):
        Cl = self.compute_Cl(alpha)
        return self.Cd0 + self.K * (Cl * Cl)

# ---------------------- Simulator ----------------------
class Simulator:
    def __init__(self, aircraft):
        pygame.init()
        self.width = 1200
        self.height = 800
        self.screen = pygame.display.set_mode((self.width, self.height))
        pygame.display.set_caption("Python Flight Simulator (no images)")
        self.clock = pygame.time.Clock()
        self.font = pygame.font.SysFont("Consolas", 16)
        self.bigfont = pygame.font.SysFont("Consolas", 28)
        self.aircraft = aircraft
        self.dt = 0.016  # fixed step ~60Hz
        self.time = 0.0

        # Wind vector in world-frame (north, east, down)
        self.wind = [2.0 * random.uniform(-1,1), 1.0 * random.uniform(-1,1), 0.0]

        # Ground / runway
        self.runway_y = 0.0
        self.ground_z = 0.0  # sea-level ground z=0 (we use negative z for altitude above ground)
        self.runway_length = 2000.0

        # History for trails
        self.trail = deque(maxlen=1000)

        # HUD toggle
        self.show_help = True

    def run(self):
        running = True
        while running:
            self.handle_events()
            for _ in range(1):  # allow stepping integration possibly multiple times if desired
                self.simulate_step(self.dt)
            self.draw()
            pygame.display.flip()
            self.clock.tick(60)
            self.time += self.dt

    def handle_events(self):
        ac = self.aircraft
        for ev in pygame.event.get():
            if ev.type == pygame.QUIT:
                pygame.quit(); sys.exit()
            elif ev.type == pygame.KEYDOWN:
                if ev.key == pygame.K_ESCAPE:
                    pygame.quit(); sys.exit()
                elif ev.key == pygame.K_h:
                    self.show_help = not self.show_help

        keys = pygame.key.get_pressed()
        # Throttle
        if keys[pygame.K_r]:
            ac.throttle += 0.6 * self.dt
        if keys[pygame.K_f]:
            ac.throttle -= 0.6 * self.dt
        ac.throttle = clamp(ac.throttle, 0.0, 1.0)

        # Elevator (pitch)
        ac.elevator = 0.0
        if keys[pygame.K_w]:
            ac.elevator = -1.0   # push nose down (convention)
        if keys[pygame.K_s]:
            ac.elevator = 1.0    # pull up

        # Aileron (roll)
        ac.aileron = 0.0
        if keys[pygame.K_a]:
            ac.aileron = -1.0
        if keys[pygame.K_d]:
            ac.aileron = 1.0

        # Rudder (yaw)
        ac.rudder = 0.0
        if keys[pygame.K_q]:
            ac.rudder = -1.0
        if keys[pygame.K_e]:
            ac.rudder = 1.0

        # Brakes
        self.brake = keys[pygame.K_SPACE]

    def simulate_step(self, dt):
        ac = self.aircraft
        # If crashed, do nothing except simple physics
        if ac.crashed:
            # simple free-fall with drag
            ac.vel[2] += ac.gravity * dt
            ac.pos = vec_add(ac.pos, vec_scale(ac.vel, dt))
            return

        # Compute wind-relative velocity (world frame)
        v_rel = vec_sub(ac.vel, self.wind)  # air relative to earth is wind; aircraft vel minus wind = airspeed vector
        speed = vec_length(v_rel)
        # Body axes: for aerodynamic calculations it's convenient to express velocity in body frame
        R = rotation_matrix(ac.phi, ac.theta, ac.psi)
        # compute body-relative velocity (inverse rotate world->body by using transpose)
        # Because R transforms body->world, world->body is R^T
        body_v = [
            R[0][0]*v_rel[0] + R[1][0]*v_rel[1] + R[2][0]*v_rel[2],
            R[0][1]*v_rel[0] + R[1][1]*v_rel[1] + R[2][1]*v_rel[2],
            R[0][2]*v_rel[0] + R[1][2]*v_rel[1] + R[2][2]*v_rel[2]
        ]
        # Angle of attack ~ arctan(w / u) where u is forward, w is vertical (body frame z down)
        u = body_v[0] if abs(body_v[0])>1e-6 else 1e-6
        w = body_v[2]
        alpha = math.atan2(-w, u)   # negative because our body z points downward in rotation matrix (see convention)
        beta = math.atan2(body_v[1], u)   # sideslip

        # Dynamic pressure
        q_dyn = 0.5 * ac.rho * speed * speed

        # Lift & drag coefficients
        Cl = ac.compute_Cl(alpha)
        Cd = ac.compute_Cd(alpha)

        # Lift and drag magnitudes (lift acts perpendicular to velocity in plane formed by forward & vertical)
        Lift = Cl * q_dyn * ac.wing_area
        Drag = Cd * q_dyn * ac.wing_area

        # Stall indication
        stalled = abs(alpha) > ac.alpha_stall

        # Direction of lift (approx): in body frame lift is upward (negative z) and perpendicular to velocity
        # Compute unit forward vector in body-frame (approx)
        if speed > 1e-6:
            u_body = [body_v[0]/speed, body_v[1]/speed, body_v[2]/speed]
        else:
            u_body = [1.0, 0.0, 0.0]

        # Lift direction: rotate forward vector by +90 deg about body Y axis -> points negative z
        lift_dir_body = [-u_body[2], 0.0, u_body[0]]
        # Normalize
        lmag = vec_length(lift_dir_body)
        if lmag < 1e-6:
            lift_dir_body = [0.0, 0.0, -1.0]
        else:
            lift_dir_body = vec_scale(lift_dir_body, 1.0/lmag)

        # Drag direction opposite to relative velocity
        drag_dir_body = vec_scale(u_body, -1.0)

        # Total aerodynamic force in body frame
        F_lift_b = vec_scale(lift_dir_body, Lift)
        F_drag_b = vec_scale(drag_dir_body, Drag)

        # Thrust (body forward)
        thrust = ac.thrust_max * ac.throttle
        if self.brake:
            thrust -= ac.thrust_max * 0.8
        F_thrust_b = [thrust, 0.0, 0.0]

        # Weight in world frame
        F_weight_w = [0.0, 0.0, ac.mass * ac.gravity]

        # Convert aerodynamic forces body->world using R
        Fa_world = mat_vec_mul(R, vec_add(F_lift_b, F_drag_b))
        Ft_world = mat_vec_mul(R, F_thrust_b)

        # Sum forces in world
        F_total = vec_add(vec_add(Fa_world, Ft_world), F_weight_w)

        # Acceleration world
        a_world = vec_scale(F_total, 1.0 / ac.mass)

        # Integrate linear motion (semi-implicit Euler)
        ac.vel = vec_add(ac.vel, vec_scale(a_world, dt))
        ac.pos = vec_add(ac.pos, vec_scale(ac.vel, dt))

        # Moments (approximate)
        # Aerodynamic pitching moment from elevator: proportional to elevator deflection * dynamic pressure * area * chord
        Mx_roll = ac.aileron_effect * ac.aileron * q_dyn * ac.wing_area * ac.wing_span * 0.01
        My_pitch = ac.elevator_effect * ac.elevator * q_dyn * ac.wing_area * ac.chord * 0.02
        Mz_yaw = ac.rudder_effect * ac.rudder * q_dyn * ac.wing_area * ac.wing_span * 0.005

        # Add damping terms (aero damping from angular rates)
        damping_p = -0.5 * ac.p
        damping_q = -0.5 * ac.q
        damping_r = -0.5 * ac.r

        Mx = Mx_roll + damping_p
        My = My_pitch + damping_q
        Mz = Mz_yaw + damping_r

        # Angular accelerations
        pdot = Mx / ac.Ix
        qdot = My / ac.Iy
        rdot = Mz / ac.Iz

        # Integrate angular rates
        ac.p += pdot * dt
        ac.q += qdot * dt
        ac.r += rdot * dt

        # Integrate Euler angles with body rates (small-angle approx using standard kinematic equations)
        # phi_dot = p + q*sin(phi)*tan(theta) + r*cos(phi)*tan(theta)
        # theta_dot = q*cos(phi) - r*sin(phi)
        # psi_dot = q*sin(phi)/cos(theta) + r*cos(phi)/cos(theta)
        phi_dot = ac.p + ac.q * math.sin(ac.phi) * math.tan(ac.theta) + ac.r * math.cos(ac.phi) * math.tan(ac.theta)
        theta_dot = ac.q * math.cos(ac.phi) - ac.r * math.sin(ac.phi)
        psi_dot = ac.q * math.sin(ac.phi) / max(math.cos(ac.theta), 1e-3) + ac.r * math.cos(ac.phi) / max(math.cos(ac.theta), 1e-3)

        ac.phi = wrap_angle(ac.phi + phi_dot * dt)
        ac.theta = wrap_angle(ac.theta + theta_dot * dt)
        ac.psi = wrap_angle(ac.psi + psi_dot * dt)

        # Simple ground collision and bounce/crash
        altitude = -ac.pos[2]  # because pos z is negative above ground
        if altitude <= 0.0:
            # On or below ground
            # Simple normal reaction
            if not ac.on_ground:
                # Touchdown logic: if vertical speed > threshold or speed too high -> crash
                if ac.vel[2] > 5.0 or speed > 60.0 or abs(ac.phi) > math.radians(45):
                    ac.crashed = True
                else:
                    ac.on_ground = True
                    ac.landed = True
                    # zero vertical speed, damp horizontal
                    ac.vel[2] = 0.0
                    ac.vel[0] *= 0.7
                    ac.vel[1] *= 0.7
            else:
                # While on ground apply rolling friction and brakes
                ac.pos[2] = 0.0
                # thrust along forward vector but friction reduces
                ac.vel[0] *= 0.995
                ac.vel[1] *= 0.995
                ac.vel[2] = 0.0
                # If throttle applied and speed enough, rotate back into flight
                ground_speed = math.sqrt(ac.vel[0]*ac.vel[0] + ac.vel[1]*ac.vel[1])
                if ac.throttle > 0.6 and ground_speed > 10.0:
                    # become airborne
                    ac.on_ground = False
                    ac.pos[2] = -0.1

        # add trail
        self.trail.appendleft((ac.pos[0], ac.pos[1], -ac.pos[2], speed, stalled))

# ---------------------- Drawing / HUD ----------------------
    def draw(self):
        screen = self.screen
        screen.fill((10, 10, 30))
        ac = self.aircraft

        # Simple horizon: draw center line and rotate according to roll and pitch
        cx = self.width // 3
        cy = self.height // 2
        # horizon angle is roll
        angle_deg = -math.degrees(ac.phi)
        pitch_pix = int(math.degrees(ac.theta) * 4.0)
        # sky
        pygame.draw.rect(screen, (80, 140, 220), (0, 0, self.width, cy + pitch_pix))
        # ground
        pygame.draw.rect(screen, (90, 60, 30), (0, cy + pitch_pix, self.width, self.height - (cy + pitch_pix)))
        # horizon line
        pygame.draw.line(screen, (255,255,255), (0, cy + pitch_pix), (self.width, cy + pitch_pix), 1)
        # tilt horizon by drawing rotated rect
        horizon_surf = pygame.Surface((self.width, 4), pygame.SRCALPHA)
        pygame.draw.rect(horizon_surf, (255,255,255,200), (0,0,self.width,4))
        rotated = pygame.transform.rotate(horizon_surf, angle_deg)
        rrect = rotated.get_rect(center=(cx, cy))
        screen.blit(rotated, rrect.topleft)

        # Draw aircraft indicator (simple)
        ac_x = cx
        ac_y = cy
        pygame.draw.polygon(screen, (230,230,230), [(ac_x-20, ac_y+6), (ac_x+20, ac_y+6), (ac_x, ac_y-12)])

        # Draw mini top-down map for position
        map_x = self.width - 360
        map_y = 20
        map_w = 340
        map_h = 200
        pygame.draw.rect(screen, (20,20,20), (map_x, map_y, map_w, map_h))
        pygame.draw.rect(screen, (100,100,100), (map_x, map_y, map_w, map_h), 2)
        # runway representation
        rr = map_x + 20
        rt = map_y + map_h//2 - 8
        pygame.draw.rect(screen, (50,50,50), (rr, rt, map_w-40, 16))
        # aircraft pos on map (scaled)
        # we center map at runway center
        scale = 0.05
        ax = map_x + map_w//2 + int(self.aircraft.pos[1]*scale)
        ay = map_y + map_h//2 - int(self.aircraft.pos[0]*scale)
        pygame.draw.circle(screen, (255,200,0), (ax, ay), 5)
        pygame.draw.line(screen, (255,255,255), (ax, ay), (ax + int(math.cos(ac.psi)*12), ay - int(math.sin(ac.psi)*12)), 2)

        # Draw trail small dots on the main view (projected)
        for i, (x,y,alt,speed,stalled) in enumerate(list(self.trail)[:200]):
            screen_x = int(cx + (y - ac.pos[1]) * 0.5)
            screen_y = int(cy + (ac.pos[0] - x) * 0.5)
            if 0 <= screen_x < self.width and 0 <= screen_y < self.height:
                color = (255,100,100) if stalled else (200,200,200)
                pygame.draw.circle(screen, color, (screen_x, screen_y), 1)

        # Instrument cluster (right side)
        ix = self.width//2 + 80
        iy = 40
        line_h = 22

        altitude = -ac.pos[2]
        speed = vec_length(vec_sub(ac.vel, self.wind))
        text_lines = [
            f"Time: {self.time:6.1f} s",
            f"Speed (TAS): {speed:6.1f} m/s ({speed*1.94384:5.1f} kt)",
            f"Altitude: {altitude:6.1f} m",
            f"Throttle: {ac.throttle*100:5.0f} %",
            f"Pitch: {math.degrees(ac.theta):6.2f} deg",
            f"Roll: {math.degrees(ac.phi):6.2f} deg",
            f"Yaw: {math.degrees(ac.psi):6.2f} deg",
            f"p,q,r: {ac.p:6.2f},{ac.q:6.2f},{ac.r:6.2f} rad/s",
            f"On ground: {ac.on_ground}  Landed: {ac.landed}  Crashed: {ac.crashed}",
        ]
        for i, line in enumerate(text_lines):
            surf = self.font.render(line, True, (230,230,230))
            screen.blit(surf, (ix, iy + i*line_h))

        # stall alert
        stalled = abs(math.atan2(- (mat_vec_mul(rotation_matrix(ac.phi, ac.theta, ac.psi), vec_sub(ac.vel, self.wind))[2]),
                                 max(1e-6, mat_vec_mul(rotation_matrix(ac.phi, ac.theta, ac.psi), vec_sub(ac.vel, self.wind))[0]))) > ac.alpha_stall
        if stalled:
            warning = self.bigfont.render("STALL!", True, (255,50,50))
            screen.blit(warning, (50,50))

        # help
        if self.show_help:
            help_lines = [
                "Controls:",
                "W/S - Elevator (pitch)",
                "A/D - Ailerons (roll)",
                "Q/E - Rudder (yaw)",
                "R/F - Throttle up/down",
                "Space - Brakes",
                "H - Toggle help",
                "Esc - Quit"
            ]
            hx = 20
            hy = self.height - 22*len(help_lines) - 20
            for i, l in enumerate(help_lines):
                surf = self.font.render(l, True, (220,220,220))
                screen.blit(surf, (hx, hy + i*20))

        # small footer
        footer = self.font.render("Python Flight Sim — realistic-ish aerodynamics, no images", True, (150,150,150))
        screen.blit(footer, (10, self.height - 22))

# ---------------------- Main ----------------------
if __name__ == "__main__":
    ac = Aircraft()
    sim = Simulator(ac)
    sim.run()
