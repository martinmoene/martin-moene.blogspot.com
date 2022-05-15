How to install Visual Studio 6 on Windows 8.1
==============================================

To install Visual Studio 6 on a computer running Windows 8.1 you need to take several special steps to succeed. Here I describe what I did to install Visual Studio 6 SP6 on Windows 8.1. 

**Contents**  
1. [Preparation](#preparation)  
2. [Install Visual Studio 6](#install)  
3. [Prevent compatibility check interception](#compatibility)  
4. [Enable using msdev from the command line](#commandline)  
5. [Create a VC6 command prompt](#prompt)  
6. [Fix resource editor](#fixre)  
7. [Wrap up](#wrapup)  
8. [Notes and references](#references)  

The  procedure uses the nice *Visual Studio 6.0 Installer* by Giorgio Brausi [1][2]. Without using it, the installation didn't finish properly and no uninstall entry was created. 
 
![](https://github.com/martinmoene/martin-moene.blogspot.com/blob/master/How%20to%20install%20Visual%20Studio%206%20on%20Windows%208.1/VisualStudio6.0Installer-v2.1.png?raw=true)

<br>
Note: many default actions are omitted from the following description.

<a name="preparation"></a>
Preparation
-------------
- Download and unzip [Visual Studio 6.0 Installer](http://nuke.vbcorner.net/Articles/VB60/VisualStudio6Installer/tabid/93/language/en-US/Default.aspx) [1][2]
- Start *Visual Studio 6.0 Installer* and read its help `?`
- Copy contents of Visual Studio Professional / Enterprise CDs to hard disk under a common folder, for example [3]: 
	- .../VS6/[EN_VS60SP6](http://www.microsoft.com/en-us/download/details.aspx?id=9183)
	- .../VS6/MSDN CD1
	- .../VS6/MSDN CD2
	- .../VS6/VSE600ENU1
	- .../VS6/VSE600ENU2
	- .../VS6/VSE600ENU3
- In the next steps, *Visual Studio 6.0 Installer* will prepare the folders to perform the installation from.
- Specify the root folder where to prepare the folders to install from via `File > Set Root folder...`;  
	For me this is `D:\Users\Martin\VS6-ROOT`
- Click `Step 1: Prepare Visual Studion 6.0 ENT` and specify the path to CD #1 of the Visual Studio 6.0 CD you collected above, `.../VS6/VSE600ENU1`. 
- Click `Step 2: Prepare MSDN Library` and specify the path to CD #1 of the MSDN Library CD you collected above, `.../VS6/MSDN CD1`.
- Click `Step 3: Prepare Service Pack 6` and specify the path to the folder (CD)with Service Pack 6 you collected above, `.../VS6/EN_VS60SP6`.

Further preparation:
- Create folder `C:\Program Files (uac)`  
	The Visual Studio development environment files will be installed here to prevent the [UAC](http://en.wikipedia.org/wiki/User_Account_Control) to intervene [4]. That would happen when Visual Studio would be installed to `C:\Program Files (x86)` [4].
- Make sure that `Microsoft Visual Studio` folders from previous *Visual Studio 6* installations are removed as well as its registry entries [5].


<a name="install"></a>
Install Visual Studio 6
------------------------

- Start the *Visual Studio 6.0 Installer*
- Note: in section Preparation above we already specified the root folder with prepared installation CDs via `File > Set Root folder...` (`D:\Users\Martin\VS6-ROOT`).
- Select Visual Studio  6.0 Enterprise (or Professional).
- Click `Install Visual Studio 6.0 ENT`
	- Choose folder for common files:  
		**`C:\Program Files (uac)`**`\Microsoft Visual Studio\Common`
	- Select desired products  
		**Note:** For Windows8.x, from Data Access main option, disable the item: ADO, RDS and OLE DB Providers  
	- Restart Windows
	- Deselect *Install MSDN*, don't install anything further now
	- Deselect *Register Now*, press Finish
- Click `Install MSDN Library`
	- Accept several defaults
	- Press `Full` button
- Click `Install Service Pack 6`
	- Accept several defaults
	- Press `Full` button

<a name="compatibility"></a>
Prevent compatibility check interception
------------------------------------------
Trying to run MSDEV.EXE now will result in a *Microsoft (R) Developer Studio has stopped working* dialog. To cure this:

- Rename `MSDEV.EXE` to `MSDEV8.EXE` [5]  
	`MSDEV.EXE` is located in C:\Program Files (x86)\Microsoft Visual Studio\Common\MSDev98\Bin.
- On `MSDEV8.EXE`, change Properties, Compatibility to: *Windows 7*:

	![](https://github.com/martinmoene/martin-moene.blogspot.com/blob/master/How%20to%20install%20Visual%20Studio%206%20on%20Windows%208.1/MSDEV8.EXE-Properties.png?raw=true)


<a name="commandline"></a>
Enable using msdev from the command line
-------------------------------------------
To enable using `MSDEV8.EXE` under the original name from the command line I created program [msdev.cpp](https://github.com/martinmoene/martin-moene.blogspot.com/blob/master/How%20to%20install%20Visual%20Studio%206%20on%20Windows%208.1/msdev.cpp). 
```C++
#include <process.h>

const char* path = "C:\\Program Files (uac)\\Microsoft Visual Studio\\Common\\MSDev98\\Bin\\MSDEV8.EXE";

int main( int argc, char * argv[] )
{
    return spawnvp( P_WAIT, path, argv );
}

// cl -nologo -EHsc msdev.cpp
```

Compile this program with VC6 or higher and copy it to folder `C:\Program Files (uac)\Microsoft Visual Studio\Common\MSDev98\Bin`.

<a name="prompt"></a>
Create a VC6 command prompt
----------------------------
To enable opening a command prompt for Visual C++ 6, we create a new shortcut.
- On the desktop, context menu, select `New > Shortcut`.
- Location of item: `%comspec% /k "C:\Program Files (uac)\Microsoft Visual Studio\VC98\Bin\VCVARS32.bat"`
- Name for this shortcut: `VC6 Command Prompt`
- Press Finish

Now move this shortcut to the start menu folder `Microsoft Visual Studio 6.0`:
- `C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Microsoft Visual Studio 6.0`

As an example, start VC6 command prompt and issue `msdev /?`:

```
Setting environment for using Microsoft Visual C++ tools.
prompt>msdev /?
Usage:
  MSDEV [myprj.dsp|mywksp.dsw]  - load project/workspace
        [<filename>]            - load source file
        /?                      - display usage information
        /EX <macroname>         - execute a VBScript macro
        /OUT <filename>         - redirect command line output to a file
        /USEENV                 - ignore tools.options.directories settings
        /MAKE [<target>] [...]  - build specified target(s)
              [<project> - <platform> <configname>]
              [[<project>|ALL] - [DEBUG|RELEASE|ALL]]
              /CLEAN            - delete intermediate files but don't build
              /REBUILD          - clean and build
              /NORECURSE        - don't build dependent projects

prompt>
```

<a name="fixre"></a>
Fix resource editor
---------------------
In [issue #8](https://github.com/martinmoene/martin-moene.blogspot.com/issues/7) of 15 May 2022, [@anzz1](https://github.com/anzz1) describes how to fix the memory allocation error in the resource editor `devres.pkg` that leads it to crash.

<a name="wrapup"></a>
Wrap up
---------
If you want to do the installation all over again and need to remove (part of) the Visual Studio installation by hand, reference [6] may be helpful to see which registry items may need removal.

I also installed WinHlp for Windows 8.1, however you may not need it at all [7].


<a name="references"></a>
Notes and references
----------------------
[1] Giorgio Brausi. [Visual Studio 6.0 Installer](http://nuke.vbcorner.net/Articles/VB60/VisualStudio6Installer/tabid/93/language/en-US/Default.aspx).

[2] Giorgio Brausi. [Visual Studio 6.0 Installer v.2.1 Last update january 18, 2014 ](https://www.youtube.com/watch?v=BPGSXTgvFNU). YouTube.

[3] Microsoft. [Visual Studio SP6](http://www.microsoft.com/en-us/download/details.aspx?id=9183).

[4] Wikipedia. [User Account Control (UAC)](http://en.wikipedia.org/wiki/User_Account_Control).

[5] Microsoft Community. [UAC and one program used very regulary](http://answers.microsoft.com/en-us/windows/forum/windows_vista-security/uac-and-one-program-used-very-regulary/67bfc4b5-faff-4de4-be48-f395bf1c519d). gbswales, 12/4/2011.

[6] Microsoft. [How To Manually Uninstall Visual Studio with MSDN Library](http://support.microsoft.com/kb/2486971)

[7] Microsoft. [Windows Help program (WinHlp32.exe) for Windows 8.1](http://www.microsoft.com/en-us/download/details.aspx?id=40899)

<!--
[x] Code Project. [How to install Visual Studio 6 on Windows 7 professional 64bit](http://www.codeproject.com/Tips/408806/How-to-install-Visual-Studio-6-on-Windows-7-profes).
-->
