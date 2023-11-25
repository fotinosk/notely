# Notely
### A CLI note taking tool

Notely allows you to quickly and simply take notes in about your work on your projects

### Usage
```
usage: notes [--init] [-h | --help] [-f | --folder] [-l | --list] [<notes>]
```
Here are some example uses:

	notes --init     Initialize project in this directory
 
	notes <note>      Write note to the root project directory
 
	notes  -f <note>  Write note to the current project subdirectory
 
	notes  -l         List notes about current project

 ### Instalation
 Note: This has not been throroughly tested with all C++ compilers therefore experiences may vary. 

Compile the application:
```bash
make
```
Install binary (moving it to your usr/local/bin) - admin password required
```bash
make install
```
