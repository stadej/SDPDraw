# SDP Draw Project Documentation

SDP Draw is a project developed for the Canadian Hereford Association to automate the random draw for the Semen Donation Project.
It is desgined to take a list of eligible bulls, and a list eligible female cows, complete a draw to decide which female cows can receive semen from certain bulls, and output the results of the draw for the respective donators and junior hereford members.

For more information about the draw itself, please see the [Hereford Website](https://www.hereford.ca/youth/projects/)

This is a console application, meaning it is run directly from the system terminal.
If you unfamiliar with using the terminal, here are some instructions to help you get started:

## Running the Application

### 1. Open Command Prompt/Terminal:
For Windows
- Press Windows Key + R to open the Run program
- Type "cmd" into the dialog box and press enter
- Alternatively, you can open the Start menu and type "cmd" into the search bar and click "Command Prompt"

For macOS
- Click the Launchpad icon in the Dock, type Terminal in the search field, then click Terminal
Alternatively, open Finder, open the /Applications/Utilities folder, then double-click Terminal

### 2. Find the File Path
- If you have downloaded this project from a zipped/compressed file, copy the project folder to a different location (ie. Desktop)

For Windows
- Navigate to the SDP project folder in your system's files
- Right click on any of the files in the SDP project folder and select "Properties"
- In the properties window, copy the file path listed next to "Location"

For macOS
- Open Finder and navigate to the SDP project folder in your system's files
- Choose View > Show Path Bar, or press the Option key to show the path bar momentarily
- Control-click the folder in the path bar, then choose Copy “folder” as Pathname

### 3. Navigate to the Working Directory
- Use the cd command to navigate to the file folder where the application is located
- For example if the path the the working directory is "C:\Users\jamie\Documents\SDP" enter "cd C:\Users\jamie\Documents\SDP" into the terminal

### 4. Run the Program
For Windows
- enter "sdpdraw.exe" into the terminal

For macOS
- enter "./sdpdraw.exe" into the terminal

### 5. Enter Inputs
- When prompted, enter the names of the bull list and female list input files in csv format
- Alternatively, the file names can be entered as command line arguments to skip the prompts, for example "sdpdraw.exe bull_list.csv female_list.csv" will accept both listed files

### 6. View Outputs
- The program will create two new output files, named donator_output.csv and junior_output.csv in the SDP project folder, which can be opened in your systems files

## Program Inputs

The program takes two .csv files as input, one containing the list of eligible bulls, and the other containing the list of females.
A .csv file can be created from an Excel spreadsheet by clicking File and Save As, then selecting "CSV UTF-8 (Comma delimited) (*.csv)" as the file type and clicking Save.
Alternatively, a Google Sheets spreadsheet also can be downloaded as a .csv file by clicking File then Download then "Comma Separated Values (.csv)"

For this project, the list of female cows is intended to be compiled with a Google Form filled out by each of the junior applicants. The results from this form can be viewed in Google Sheets and downloaded as a .csv file from there.
[Here](https://docs.google.com/forms/d/e/1FAIpQLSeXtZBJTWvQdJZMzGYNYyWk_k1SfnkxcWexS9T08jI3s2tvtQ/viewform) is an example from last year's draw.

Here is what should be included in each input file:

### Bull List
| Bull Name | Donator Name | Donator Email | Straws |
| --------- | ------------ | ------------- | ------ |
| ...       | ...          | ...           | ...    |
| ...       | ...          | ...           | ...    |

### Female List
| Female Name | Member Name | Member Email | Straws | Bull 1 | Bull 2 | Bull 3 | Bull 4 | Bull 5 | Bull 6 |
| ----------- | ----------- | ------------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | 
| ...         | ...         | ...          | ...    | ...    | ...    | ...    | ...    | ...    | ...    |
| ...         | ...         | ...          | ...    | ...    | ...    | ...    | ...    | ...    | ...    |

## Program Outputs

The program will create or overwrite two output files, donator_output.csv and junior_output.csv
Here is what is included in each output file:

### donator_output.csv
This record is generated for each donator in the draw
The table includes all junior members receiving semen from the draw as well as which bull they received it from and how many straws
| Result for  | (Donator Name) | at (Donator Email) |
| ----------- | -------------- | ------------------ |
| Junior Name | Bull Name      | Number of Straws   |
| ...         | ...            | ...                |
| ...         | ...            | ...                |

### junior_output.csv
This record is generated for each junior member in the draw
The table includes all the female cows submitted for the draw, which bull they received semen from (if any) as well as the Email of that bull's donator
| Result for  | (Member Name)  | at               | (Member Email) |
| ----------- | -------------- | ---------------- | -------------- |
| Female Name | Bull Name      | Number of Straws | Donator Email  |
| ...         | ...            | ...              | ...            |
| ...         | ...            | ...              | ...            |





