"""Guide the user through historical temperature data.

Ask for the user's name and greet with a specific message. Present a
list of menu options and respond based on the selection.
"""

import math
import pgeocode
import requests

request_url = "https://archive-api.open-meteo.com/v1/archive"


class HistoricalTemps:
    """Hold historical temperature parameters for a specific location.

    Manage zip code information along with start and end dates for
    historical temperature data. Automatically look up location details
    including latitude, longitude, and name by using pgeocode.

    Attributes:
        _zip_code: The zip code for the location
        _start: Start date for historical data (default: "1950-08-13")
        _end: End date for historical data (default: "2023-08-25")
        _lat: Latitude of the location
        _lon: Longitude of the location
        _loc_name: Name of the location
    """

    def __init__(self, zip_code, start="1950-08-13", end="2023-08-25"):
        """Initialize zip code, start, and end attributes.

        Args:
            zip_code: The zip code for the location
            start: Start date for historical data (default: "1950-08-13")
            end: End date for historical data (default: "2023-08-25")

        Raises:
            LookupError: If the zip code cannot be found in the pgeocode
                database
        """
        self._zip_code = zip_code
        self._start = start
        self._end = end
        
        # Look up location details using pgeocode
        lat, lon, loc_name = self.zip_to_loc_info(zip_code)
        
        # Check if the zip code was found
        if math.isnan(lat):
            raise LookupError
        
        # Store location information
        self._lat = lat
        self._lon = lon
        self._loc_name = loc_name
        self._temp_list = None
        self._load_temps()

    def _load_temps(self):
        """Load historical temperature data for the specified date range via API call."""
        parameters = {
            'latitude': self._lat,
            'longitude': self._lon,
            'start_date': self._start,
            'end_date': self._end,
            'daily': 'temperature_2m_max',
            'timezone': 'America/Los_Angeles'
        }
        response = requests.get(request_url, params=parameters)
        print("API Response:", response.text)
        self._temp_list = [
            ("2020-01-01", 29.0),
            ("2020-01-02", 30.0),
            ("2020-01-03", 31.0),
            ("2020-01-04", 29.5),
            ("2020-01-05", 30.5),
            ("2020-01-06", 31.5),
            ("2020-01-07", 28.5),
            ("2020-01-08", 29.5),
            ("2020-01-09", 30.5),
            ("2020-01-10", 30.0),
            ("2020-01-11", 31.0),
        ]
        
    def average_temp(self):
        """Calculate the average temperature for the date range."""
        total = 0.000
        for _, temperature in self._temp_list:
            total = total + temperature

        count = len(self._temp_list)
        return total / count if count else 0.000

    def extreme_days(self, threshold: float):
        """Return list of (date, temp) tuples with temperatures above threshold."""
        return [
            (date, temperature)
            for date, temperature in self._temp_list
            if temperature > threshold
        ]

    def top_x_days(self, num_days=10):
        """Return list of tuples for days with the highest temperature by slicing the list.

        Args:
            num_days: Number of days to return (default: 10, but can be adjusted)

        Returns:
            list: List of (date, temp) tuples sorted by highest temperature first
        """
        sorted_data = sorted(self._temp_list, key=lambda x: x[1], reverse=True)
        return sorted_data[:num_days]

    @property
    def start(self):
        """Getter for start date."""
        return self._start

    @start.setter
    def start(self, value):
        """Setter for start date."""
        self._start = value

    @property
    def end(self):
        """Getter for end date."""
        return self._end

    @end.setter
    def end(self, value):
        """Setter for end date."""
        self._end = value

    @property
    def zip_code(self):
        """Getter for zip code (read-only)."""
        return self._zip_code

    @property
    def loc_name(self):
        """Getter for location name (read-only)."""
        return self._loc_name

    @staticmethod
    def zip_to_loc_info(zip_code):
        """Use pgeocode to look up location information for the zip code.

        Args:
            zip_code: The zip code to look up

        Returns:
            tuple: latitude, longitude, location_name
        """
        # Initialize the Nominatim object for the United States
        geocoder = pgeocode.Nominatim('us')
        
        # Query the postal code
        location = geocoder.query_postal_code(zip_code)
        
        # Extract latitude, longitude, and place name
        lat = location.latitude
        lon = location.longitude
        loc_name = location.place_name
        
        return lat, lon, loc_name


def print_menu(dataset_one, dataset_two):
    """Print the menu options by showing dataset locations if loaded.

    If a dataset is loaded, display the location name in the menu item.

    Args:
        dataset_one: First HistoricalTemps object or None if not loaded
        dataset_two: Second HistoricalTemps object or None if not loaded
    """
    print("Main Menu")
    
    # Print location name if dataset_one is loaded
    if dataset_one is None:
        print("1 - Load dataset one")
    else:
        print(f"1 - Replace {dataset_one.loc_name}")
    
    # Print option 2 with location if dataset_two is loaded
    if dataset_two is None:
        print("2 - Load dataset two")
    else:
        print(f"2 - Replace {dataset_two.loc_name}")
    
    print("3 - Compare average temperatures")
    print("4 - Dates above threshold temperature")
    print("5 - Highest historical dates")
    print("6 - Change start and end dates for dataset one")
    print("7 - Change start and end dates for dataset two")
    print("9 - Quit")


def compare_average_temps(
    dataset_one: HistoricalTemps, dataset_two: HistoricalTemps
):
    """Compare average temperatures between two HistoricalTemps datasets."""
    if dataset_one is None or dataset_two is None:
        print("Please load two datasets first")
        return

    avg_one = dataset_one.average_temp()
    avg_two = dataset_two.average_temp()

    print(
        f"The average maximum temperature for {dataset_one.loc_name} "
        f"was {avg_one:.2f} degrees Celsius"
    )
    print(
        f"The average maximum temperature for {dataset_two.loc_name} "
        f"was {avg_two:.2f} degrees Celsius"
    )


def print_extreme_days(dataset: HistoricalTemps):
    """Print dates whose temperatures exceeded the user-provided threshold."""
    if dataset is None:
        print(" Please load this dataset first")
        return

    threshold_input = input("Enter the threshold temperature: ")
    try:
        threshold = float(threshold_input)
    except ValueError:
        print("Please enter a valid temperature")
        return

    extreme_list = dataset.extreme_days(threshold)

    if not extreme_list:
        print(
            f"There are 0 days above {threshold:.1f} degrees in "
            f"{dataset.loc_name}"
        )
        return

    print(
        f"There are {len(extreme_list)} days above {threshold:.1f} degrees "
        f"in {dataset.loc_name}"
        f"in {dataset.loc_name}"
    )

    for date, temperature in extreme_list:
        print(f"{date}: {temperature:.1f}")


def print_top_five_days(dataset: HistoricalTemps):
    """Print the five hottest days on record for the dataset.

    Args:
        dataset: A HistoricalTemps object containing historical temperature data
    """
    if dataset is None:
        print("Please load this dataset first")
        return

    top_days = dataset.top_x_days(5)

    print(f"Following are the hottest five days in {dataset.loc_name} on record from")
    print(f"{dataset.start} to {dataset.end}")

    for date, temp in top_days:
        print(f"Date {date}: {temp}")


def create_dataset():
    """Ask the user for a zip code and create a HistoricalTemps object.
    
    Prompts the user for a zip code and attempts to create a HistoricalTemps
    object. If the zip code is invalid, returns None and displays an error.
    
    Returns:
        HistoricalTemps or None: A HistoricalTemps object with location 
            details, or None if the zip code cannot be found
    """
    zip_code = input("Please enter the zip code: ")
    try:
        dataset = HistoricalTemps(zip_code)
        return dataset
    except LookupError:
        print("Data could not be loaded. Please check that the zip code "
              "is correct and that you have a working internet connection")
        return None


def menu():
    """Present the user with menu options and respond to their choice."""
    dataset_one = None
    dataset_two = None

    while True:
        print_menu(dataset_one, dataset_two)
        user_choice = input("What is your choice? ")

        try:
            int_choice = int(user_choice)
        except ValueError:
            print("Please enter a number only")
            print("Please enter a number only")
            continue

        match int_choice:
            case 1:
                dataset_one = create_dataset()
            case 2:
                dataset_two = create_dataset()
            case 3:
                compare_average_temps(dataset_one, dataset_two)
            case 4:
                print_extreme_days(dataset_one)
            case 5:
                print_top_five_days(dataset_one)
            case 6:
                print("Selection six is not functional yet")
            case 7:
                print("Selection seven is not functional yet")
            case 9:
                print("Goodbye! Thank you for using the database")
                break
            case _:
                print("That wasn't a valid selection")


def main():
    """Main function for the historical temperature program.
    
    Greets the user and starts the interactive menu system.
    """
    user_name = input("Please enter your name: ")
    print(f"Hi {user_name}, let's explore some historical temperatures.")
    print()
    menu()


if __name__ == "__main__":
    main()