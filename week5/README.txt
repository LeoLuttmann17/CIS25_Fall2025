# Leo's Password Maker

This is a small C++ program I wrote to generate passwords. When you run
it, it greets you and asks how strong you want your password to be.

## What It Does

-   **Strong (1)** -- Makes a 7-character password with letters,
    numbers, and symbols. It always has at least two numbers and two
    special characters.
-   **Medium (2)** -- Asks for your name, your birth year, and a number
    of years, then uses those to create a password.
-   **Weak (3)** -- Just prints the word "password."

## How to Run It

1.  Save the code in a file called `password_maker.cpp`.

2.  Open a terminal or command prompt in the same folder.

3.  Compile it with:

    ``` bash
    g++ password_maker.cpp -o password_maker
    ```

4.  Run it:

    -   On Mac or Linux: `./password_maker`
    -   On Windows: `password_maker.exe`

## Example

    How are you?
    Welcome to Leo's Password Maker
    How strong do you want your password to be?
    1. Strong
    2. Medium
    3. Weak
    Enter your choice (1-3): 1
    Your strong password is: Bz@4q#8
