# C - Password Manager

#DISCLAIMER
I made this as a project for a university course, and, as such, it is nothing more than a prototype. This program was not tested from a security standpoint, so my recommendation is against trying to use it.
-----------------------------------------------------------------------

![image](https://user-images.githubusercontent.com/103068026/195428149-b27a11c1-0d2c-44d2-80a3-83bcb09938a7.png)

  Above is a very simple flowchart showing how the user interaction process should happen. 
  
  When the user starts the app they are greeted by the initial “Welcome Screen”. There they have the option to either authenticate, create a user ( account ) or exit. Now, obviously, a user cannot authenticate before creating an account, since they do not have anything to authenticate to ( This is something the program explains if the user attempts to do so ). When creating a user or authenticating and entering the Master Passphrase ( which is technically the key to all the passwords a user saves using the app ), the echo of the console is disabled ( nothing written appears on the screen ) - This only works in windows command prompt, as this app was initially designed on windows and was tested on windows.

  Once logged in, the user can add a password, retrieve all the passwords, retrieve a password for a specific service, delete an  entry, modify an entry, see a graph of the lengths of all his passwords, or log out ( which will take him back to the main screen ). All user files ( and backups of these files are stored in a subdirectory to the directory in which the app is executed to keep things clean and avoid cluttering. The program will check if the folder exists and if not, create it.  When modifying or deleting entries we use a temporary file, in which everything except the entry to be deleted ( if modifying an entry the modified version is copied ), and then the file itself is copied and overwrites the initial decrypted file.

  All files have backups and the app has checks in place to verify if the files needed for its operations exist. If any of the necessary files cannot be opened, then, depending on the action the program is performing, it does one of two things: 
    -	If the app is in the Create_User or Add_Entry function it will create the necessary files, since on first use these files are not guaranteed to exist ( unless, they are for some reason created by the user in advance ), so it makes sense that these functions would create the files if they do not exist and inform the user, that this is normal behavior on first use. 
    -	All of the other functions that access files will tell the user that before perfoming the requested operations they need to either create a user or add at least an entry to the passwords file ( depending on the context ).
  

#GRAPHICAL PART
----------------
  
  The Show_Graph function is a made in such a way that no matter the amount of values it takes into consideration for the graph and no matter the values themselves, the graph will never go out of bounds. The values used are normalized to a range from 0.1 to 0.9 and, to determine the length of the bars of the graph I divide 1.9 by the number of values. Since from one corner to the other of the window ( if we are talking in terms of float coordinates ) we have a length of 2 ( from -1 to 1 ) – I am using 1.9 and not 2 since, when I used 2, for some reason the graph would go out of bounds horizontally - The display of bars starts from -0.95 and stops somewhere around 0.9, for the reason mentioned above ( dividing 1.9 and not 2 by the number of values ).
  
  
  
  Sorry for the weird formatting, this is one of my first repos.
