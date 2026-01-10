# NeoPrisma / nprisma

NeoPrisma is a fast, clean, and reliable autoclicker & macro for MacOS (hopefully coming to Windows soon).
It is the successor to Prism's Autoclicker 4.0.

| | nprisma | prism's autoclicker |
| - | - | - |
| Autoclick | <ul><li>- [x] Fixed at ~900CPS & left click </li></ul> | <ul><li>- [x] Supports autoclicking LMB, RMB, and most keys. </li></ul> |
| Tasks | <ul><li>- [x] Full support, including mouse drag events. </li></ul> | <ul><li>- [x] Supports saving/loading to files, recording, and playback. </li></ul> |
| Interface | <ul><li>- [x] Minimal system tray UI. </li></ul> | <ul><li>- [x] Very simple GUI using TkInter. </li></ul> |

## Building

neoprisma must be built manually. You should be able to do this with fairly little trouble:
- Clone the repository. 
- `cd` to the `neoprisma` folder you just cloned
- Create a Python virtual environment and enter it.
- Install `pybind11`, `PyQt6`, and `pynput` using pip.
- cd to `neoprisma/src` and compile playback.cpp using this command: `c++ -O3 -Wall -shared -std=c++17 -undefined dynamic_lookup $(python3 -m pybind11 --includes) playback.cpp -o playback$(python3-config --extension-suffix) -framework ApplicationServices`

Once finished with those steps, run `python3 main.py` while in the venv and src directory. After granting accessibility & input monitoring permissions to your terminal, neoprisma should be running.

(sorry for making this overly complicated i swear ill make an install script and proper app)

## Hotkeys

All hotkeys are in the range of `<ctrl>+<fn>+<f7-f9>` (or `<ctrl>+<f7-f9>` if you have configured the function keys to need fn to do their special action)

`<f7>` - toggle recording
`<f8>` - toggle autoclicker
`<f9>` - toggle playback

## Known Issues

Pynput will sometimes crash due to a bug within the library, causing hotkeys to be unresponsive. Additionally, the program will sometimes get `trace trap`'d by the OS for no apparent reason when you toggle recording on. However, neoprisma is still much more stable than prism's autoclicker.

## Performance

CPU usage does not seem to be excessive, nor does battery usage.
However, neoprisma isn't optimized for either, and mostly optimized for accurate recording playback. 

When testing with a recording of Geometry Dash gameplay, neoprisma didn't do the best, getting as far as the middle of ship section after several tries. 

Setting the process priority to 20 may help.