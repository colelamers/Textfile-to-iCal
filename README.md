# Textfile-to-iCal
This is currently a Beta Windows based program that will find a file based on a specified path, parse it and spit it back out as an importable .ics file for Google calendar and Thunderbird.

Has only been set up to work with Windows directories but with some tinkering could easily work with MacOS and Linux.

Currently there are a few deprecated things I may no longer need such as the Calendar Class and I could just substituted it for a string.
Some efficiency changes can also be made but I haven't been able to figure them out yet.

To Run:

Go into main.cpp, adjust the file names and paths on line 196 and 197 to correspond to your directory and file name.
Next, follow the template of the TaskICSTemplate.txt file (this contains a few examples that show you only need a date).
Because you will need to compile it yourself based off of these parameters to have them statically be retained within the exe, you'll need to compile it and run it within whichever compiler of your choosing, or visual studio since I have the entire solution there as well.
