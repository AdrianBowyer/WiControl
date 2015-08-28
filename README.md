WiControl
=========

Wireless Control with Panstamp and Raspberry Pi

The libserial-0.6.0rc1 directory contains a version of the C++ libserial
library configured for the Raspberry Pi.

Installation:
-------------

On the Pi:

$ mkdir Panstamp
$ cd Panstamp
$ git clone https://github.com/AdrianRepRap/WiControl.git
$ cd WiControl/libserial-0.6.0rc1
$ make clean
$ sudo make install
$ cd ../wput-0.6.1
$ ./configure
$ make
$ sudo make install
$ cd ../Host-software/Debug
$ make clean
$ make
$ cd ../..
$ cp run-heat-control.sample run-heat-control

then edit run-heat-control to download your control file from your webserver and to 
run the program with that file.  Replace the ***** with your ftp password so that the 
temperatures can be uploaded to the server.  (Remember after that to keep the file
private - in particular don't push it to Github...)  Finally

$ sudo crontab -e

and add lines at the end like this:

* * * * * /home/pi/Panstamp/WiControl/run-heat-control
0 4   *   *   *    /sbin/shutdown -r +5

That will cause the shell script run-heat-control to be run once a minute, and cause a
system reboot at 04:05 in the morning to keep everything clean and tidy.  The sudo is
needed because shutdown needs root privilege.

When it runs it creates a status file called Status.

$ cat Status

will tell you what your system last did.  If you

$ touch Log

then each time run-heat-control runs it will append the status
report to the end of itself, which keeps a history and is therefore
useful for debugging.

When you have finished debugging don't forget to

$ rm Log

otherwise it will eventually fill up the SD/disc...


Adrian Bowyer

