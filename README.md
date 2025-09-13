# Steam Uploader

> [!NOTE]
> This tool is a work in progress. Please notify us of any bugs via the issues page.

> [!IMPORTANT]
> x86 architecture is NOT supported!

A CLI tool to update items on the Steam Workshop independently from the game.
Most workshop features are supported, but more are on their way!

This tool is confirmed working on:

- Linux Mint 22.1 x86_64 (Kernel: 6.8.0-71-generic; Shell: bash 5.2.21)
- Windows 10 22H2
- Windows 11 24H2

Built with the [Steamworks SDK](https://partner.steamgames.com/doc/sdk) v1.62.

---

## Usage

> [!NOTE]
> Steam needs to be open (even in the background) to use the tool!

1. Install and setup vcpkg (only follow the first two steps, skip creating the project directory) - [Install and use packages with CMake](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started)
   An example of what someone might do is this:
   
   ```bash
       cd C:\ && git clone https://github.com/microsoft/vcpkg.git
       cd vcpkg && bootstrap-vcpkg.bat
   ```
   
   Proceeded by setting `VCPKG_ROOT` environment variable and appending the same vcpkg root directory to the `PATH` environment variable.

2. Open a terminal/CLI in the program's containing folder in the folder:
   
   #### Windows
   
    In the File Explorer address bar with the breadcrums of the folder you're in, you can click the empty area and type `cmd` to open the terminal inside this folder.
    Alternatively, open the Command Prompt application and set the cwd:
   
   ```bash
       cd path/to/folder
   ```
   
    For paths with spaces, make sure to enclose them with the double-quotes `"` symbol.
   
   #### Linux / MacOS
   
    Open the respective terminal app and cd to the folder like the above codeblock.

3. Use the application! If you need some help with this, see the [examples](#examples).

## Arguments

These are the arguments for the tool.
If you need to view the short versions of these parameters, see the help page of the application via:

```bash
./SteamUploader -h
```

> [!IMPORTANT]
> The parameters listed below are mandatory.

| Argument                  | Description                                                                                        |
| ------------------------- | -------------------------------------------------------------------------------------------------- |
| `--appID appID`           | The [App ID](https://pzwiki.net/wiki/App_ID) for your game.                                        |
| `--workshopID workshopID` | The [Workshop ID](https://pzwiki.net/wiki/Workshop_ID) of your item. Overwritten by using `--new`. |

> [!IMPORTANT]
> The parameters listed below are optional, but at least one is required.
> You can use these to update specific elements of your workshop item.

| Argument                | Description                                                                                                                                                                                                                                                                                                   |
| ----------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `--description path`    | Path to a text file which holds the description. The description uses [BBCode](https://pzwiki.net/wiki/BBCode).                                                                                                                                                                                               |
| `--preview path`        | Path to a preview image or gif to upload of suggested format JPG/PNG/GIF. File size limit of 1 MiB.                                                                                                                                                                                                           |
| `--content path`        | Path to the folder of the content to upload.                                                                                                                                                                                                                                                                  |
| `--title title`         | Title of the workshop item.                                                                                                                                                                                                                                                                                   |
| `--visibility num`      | Visibility of the item on the workshop:<br/>`0` - Public (default)<br/>`1` - Friends Only<br/>`2` - Private/Hidden<br/>`3` - Unlisted                                                                                                                                                                         |
| `--tags tag1,tag2,tag3` | A comma-separated list of tags to apply for the workshop item. The tag list will be cleared if this is empty (specified by `--tags ""`).<br/>Only the tags predefined by the developers should be used. Find the list of tags on the game's workshop page via the "Browse By Tags" section on the right side. |
| `--new`                 | Creates a new workshop item and thus a new workshop ID which is used to update other fields (description, preview, etc).<br/>This parameter can be called alone which will create an empty Workshop item without anything on the Workshop page which is hidden by default.                                    |

> [!NOTE]
> The parameters listed below are optional.

| Argument                  | Description                                                                                                                                                                                                                                                                                                                                 |
| ------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `--patchNote path`        | Path to a text file which holds the patch note of the update. This is supposedly only needed if you upload new content. The patch note uses [BBCode](https://pzwiki.net/wiki/BBCode).                                                                                                                                                       |
| `--language languageCode` | Specify the target workshop language for the title and description of this upload by using the correct [API language code](https://partner.steamgames.com/doc/store/localization/languages).<br/>By default, it is set to `english`.<br/>Steam supports different languages for workshop pages based on the viewer's language set in Steam. |
| `--update`                | Updates the application to the latest release version.                                                                                                                                                                                                                                                                                      |

## Examples

Consider a workshop item `123456789` for Project Zomboid (appid 108600) that follows Project Zomboid's [mod structure](https://pzwiki.net/wiki/Mod_structure)).
Most of these examples have Linux paths. The content example has both Windows and Linux paths to refer to if you need help.

> [!NOTE]
> These examples assume the cachedir is default. If the cachedir is set to a different path, update the examples accordingly.

### Updating the content

To update the content of your mod while on Windows:

```bash
SteamUploader -a 108600 -w 123456789 -c %USERPROFILE%\Zomboid\Workshop\ExampleMod\Contents
```

And similarly for Linux:

```bash
./SteamUploader -a 108600 -w 123456789 -c ~/Zomboid/Workshop/ExampleMod/Contents
```

### Updating the preview image

To update the preview of your mod with a gif (or other image formats):

```bash
./SteamUploader -a 108600 -w 123456789 -p ~/Workshop/ExampleMod/preview.gif
```

### Updating the description

To update the description of your mod, you need to create a text file which holds the description in [BBCode](https://pzwiki.net/wiki/BBCode):

```bash
./SteamUploader -a 108600 -w 123456789 -d "~/Zomboid/Workshop/Path with space/description.txt"
```

### Updating the tags

To set the tags to "Audio" and "Build 42" (Project Zomboid's specific Workshop tags):

```bash
./SteamUploader -a 108600 -w 123456789 -T "Animals,Build 42"
```

### Updating multiple things at once

You can combine multiple arguments at once to update different things in a single command:

```bash
./SteamUploader -a 108600 -w 123456789 -c ~/Zomboid/Workshop/ExampleMod/Contents -p ~/Zomboid/Workshop/ExampleMod/preview.gif -T "Animals,Build 42"
```

---

## Building

> [!NOTE]
> Using MSVC toolchain is not fully supported, use it at your own risk! MinGW is recommended on Windows, using the `x64-mingw-static` vcpkg triplet.

The only external dependency that is required to be manually installed is curl.
Windows users should install [curl for Windows](https://curl.se/windows/).
Linux users need to add the corresponding libcurl package via the system's package manager.

Building in release mode:

```shell
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

You will want to add onto the first cmake command `-DVCPKG_TARGET_TRIPLET=x64-mingw-static` if you are on Windows and are using MinGW.

Generating release archives:

```shell
cd build
cpack -G ZIP -C Release
```

## Contact

The tool was created by me (discord: sirdoggyjvla) for the Project Zomboid modding community.
You can find us on the [Unofficial Modding Discord](https://pzwiki.net/wiki/Unofficial_Modding_Discord) of Project Zomboid.
