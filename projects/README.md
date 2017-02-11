Projects Directory
==================

The set of projects and libraries needed for the practicals

# Pre-requisites? #

* Eclipse Mars with CDT
* Android SDK and NDK and adb
* Tango Tablet
* For Arch Patrick has a detailed install guide (maybe include here)

# What is available? #

# 1. Project hello\_tango

In Eclipse go to File>Import...>Existing Android Code into Workspace.

Click Browse beside the Root Directory text box, and navigate to the root directory name HelloTango in the repository. Immediately within that folder contains the AndroidManifest.xml.  Click Ok, and then click Finish.  

From the toolbar click Run>Run Configurations.  Select the Android Application and if not already there, create a new Android application from the menu above. Select it, and in the Project textbox select browse and pick HelloTango. In the Target submenu ensure 'Always prompt to pick device' is selected.  Then click Run.

You should now see 'Hello from C++!' on the screen.

# Eclipse Guide (How I Learned to Stop Worrying and Love the Eclipse) #

# 1. Logcat

If no logcat messages appear it could be a couple of things.

Ensure that Android studio has not been running first - the two editors conflict.

If the logcat messages appear blank still, it is possibly due to a GTK bug and some configuration errors.  On Arch this has been fixed by editing /usr/lib/eclipse/eclipse.ini and adding:

    --launcher.GTK_version
    2

Just before the following line:

    --launcher.appendVmargs
	
It is then also neccesary to change your .metadata/.plugins/org.eclipse.core.runtime/.settings/com.android.ide.eclipse.ddms.prefs file within your eclipse workspace to the following.
    
	ddms.logcat.auotmonitor.level=error  
    ddms.logcat.automonitor=false  
    ddms.logcat.automonitor.userprompt=true  
    eclipse.preferences.version=1
    logcat.view.colsize.Application=169
    logcat.view.colsize.Level=54
    logcat.view.colsize.PID=54
    logcat.view.colsize.Tag=198
    logcat.view.colsize.Text=619
    logcat.view.colsize.Time=182

Finally - if you can only see the Level of the debug, scroll right and resize the log output.

# 2. Ending programs

To be able to stop programs after running in eclipse,  go to Window>Show View>Other and select Devices.

In the new Devices tab at the bottom you have a list of the running processes.  Find your one e.g. ac.uk.imperial.hellotango and click stop button at the top.

# 3. Vim support

VWrapper is a reasonable approximation to vim.  To install it got to Help>Install New Software.  Enter the following URL in the 'Work with:' textbox:

    http://vrapper.sourceforge.net/update-site/stable
	
Select Vrapper and click finish.

# 4. Stop annoying errors

For some reason eclipse really does not work well with c++ error checking.  You will often get spurious errors in projects that can build and work properly.  To disable this annoying feature, right click the project and go to properties.

Go to C/C++ General>Code Analysis>Launching and ensure both checkboxes are unticked.

# 5. libncurses.so.5 cannot open shared object file

Some systems have a newer version of libncurses, which is not supported the prebuild NDK binaries. In order to include the library download 

http://seblu.net/a/arm/packages/n/ncurses/ncurses-5.9_20141101-1-x86_64.pkg.tar.xz

Extract it and copy from

sudo cp <archive_folder>/usr/lib/libncurses.so.5.9 /usr/lib
ln -s /usr/lib/libncurses.so.5.9 /usr/lib/libncurses.so.5

then is should work

# Debugging #
Preparations:
* Open Eclipse in your project folder, open AndroidManifest.xml
* Open the Application tab and set debuggable to true
* Open jni/Android.mk and make sure that for LOCAL_CFLAGS '-g' and '-O0' is set
* Make sure your device is plugged in
* Clear your build and run your application in debug mode and the quit the program
* Run 'bash'
* Run 'export PATH=$PATH:/vol/bitbucket/bjm113/android-sdk-linux/platform-tools:/vol/bitbucket/bjm113/android-ndk-r10e' (set it in your .bashrc to make this permanent)
* cd to the project folder (where the AndroidManifest.xml is)
* Run 'ndk-gdb.py --start'
* If it runs successfully gdb then type 'quit' and hit enter
* In Eclipse go to Run -> Debug Configurations...
* Add C/C++ Remote Application, named it "LinkC++Debugging"
* In the Main tab unter C/C++ Application click Browse...
* Select in your applicaiton folder 'app_process' under <application-folder>/obj/local/armeabi-v7a/
* Select HelloTango as the Project if not already set
* Select the Debugger tab and 
* Select for GDB debugger /vol/bitbucket/bjm113/android-ndk-r10e/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gdb
* Select for GDB command file <application-folder>/obj/local/armeabi-v7a/gdb2.setup (you don't have a gdb2.setup there but put it there anyway)
* De-select stop on startup at:
* In the Connection tab set Port number to 5039
* In a console run 'cp <application-folder>/obj/local/armeabi-v7a/gdb.setup <application-folder>/obj/local/armeabi-v7a/gdb2.setup' 
* Edit gdb2.setup and remove the line with "target remote :5039"

Run Debugging:
* (run bash, then export PATH as above, if not already done)
* In Eclipse put a breakpoint in <application-folder>/src/ac.uk.imperial.hellotango/JNIinterface.java at the line with System.loadLibrary()
* Then in project set a break point and run your application as usual in debug mode
* When the application launches open a console, cd to your application folder and run ndk-gdb-android.py (this start the gdb-server on your device)
* In Eclipse click on the combo arrow next to debug symbol and select "LinkC++Debugging"
* In Eclipse hit resume an the code should stop at your selected break point
- 
