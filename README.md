# Working Hour Manager

This app was created for a friend since at his work place the hours are not right away tracked and need to be tracked manually.
It enables tracking of time / vacation days etc.

This was also used as a toy project to try out latest QT6/QML feature.

## Features

<img src="docs/Month with minutes as minutes.png">
<img src="docs/Month with minutes as fractions.png">

- Automatically generation of upcoming work weeks / months / years
- Calculation of worked hours per week / month
- Calculate earliest end time on friday. If you have your times for monday to thursday and friday start time  
  the program can tell you what is the earliest time to finnish on friday to have your required work hours of the week
- Overtime in week / month / year
- Tracking remaining vacation days
- Tracking remaining flextime days
- Display time with minutes or fraction of an hour
- Data is saved / loaded on open / close of app in json file.

&nbsp;

<img src="docs/Settings year.png">

- Settings for each year:
  - Default work time per day
  - Pause time per day
  - Flextime days count
  - Vacation days count

&nbsp;

<img src="docs/Settings general.png">

- Settings for the app:
  - Display time less than an hour in minutes or fraction

## Technologies used

- Backend: C++17 with QT 
- Frontend: QML

## Dependencies

- QT >= 6.5 
  - You can download QT [here](https://www.qt.io/download). 
- IDE: QT Creator (Done with CMake so other ides should work too)
- Only tested under Manjaro but should work cross platform

## Limitations / TODOs

- Currently in the workplace pause time is automatically calculated of each day  
  so we only set it in the settings fixed per day. In many companies pause time  
  can be logged by minute.
- Day type for half Vacation / Holiday. In germany 24.12 / 31.12 counts as half  
  workday so we need a day type which can be `0.5 workday + 0.5 holiday` or `0.5 vacation + 0.5 holiday`.


## Contributing

- Feel free to open a pull request to add new features or fix something. 
- Before adding something it is a good idea to raise an issue and discuss it.
- Follow existing code style to keep code consistent.

## License

This project is licensed under the GPL3 License - see the [LICENSE.md](LICENSE.md) file for details
