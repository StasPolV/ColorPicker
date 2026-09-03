# ColorPicker

![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus&logoColor=white)
![Qt](https://img.shields.io/badge/Qt-%E2%89%A5%206.5-41CD52?logo=qt&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%E2%89%A5%203.28-064F8C?logo=cmake&logoColor=white)
![Platform](https://img.shields.io/badge/platform-Windows-0078D6?logo=windows&logoColor=white)

A desktop screen colour picker built with **C++20** and **Qt6** (Widgets), featuring a circular magnifier that follows the cursor across the whole desktop and a global mouse grab so a pick never leaks through to the application underneath. The code is split into two static libraries: `core` reads pixels off the screen and knows nothing about the UI, `gui` draws things and knows nothing about how a colour is obtained. `main.cpp` is the only place where the two are wired together, through signals and slots.

> **Platform support:** the build is portable and the UI is plain Qt, but reliable global click detection is implemented for Windows. See [Notes](#notes) before building elsewhere.

## Features

- Pick any pixel on the desktop, including pixels belonging to other applications
- Circular magnifier that follows the cursor, with a 9×9 grid and a highlighted centre cell marking the exact pixel that will be picked
- The picked pixel is centred in the magnifier on every monitor, at every display scaling factor, and right up against the screen edges
- Global mouse grab: while picking, a click is consumed by ColorPicker instead of pressing whatever button it landed on in another program
- `Esc` cancels an active pick
- The result is shown in an embedded `QColorDialog`, so RGB, HSV, HTML notation and the colour wheel come for free
- The main window stays on top, so it remains visible while you point at another application
- Correct handling of high-DPI screens and multi-monitor setups with negative or offset coordinates

## Build

**Requirements:** C++20 compiler, CMake ≥ 3.28, Qt6 (`Core`, `Gui`, `Widgets`)

```bash
git clone <repo_url>
cd <project_folder>
cmake -S . -B build -DCMAKE_PREFIX_PATH=<path_to_Qt6>
cmake --build build -j
```

Run the `ColorPicker` executable from the `build` folder.

## Usage

1. Press **Start Grabbing**. The cursor turns into a crosshair and the magnifier appears next to it.
2. Move the pointer anywhere on the desktop and click to pick the pixel under the crosshair, or press `Esc` to cancel.
3. The picked colour appears in the colour dialog, where it can be read in any notation it supports.

## Architecture

| Component | Responsibility |
| --- | --- |
| `ScreenGrabber` | Resolves which `QScreen` the cursor is currently on |
| `ColorGrabber` | Grabs the screen, converts to `QImage`, reads the pixel under the cursor, emits `ColorGrabbed` |
| `MagnifierWidget` | Frameless translucent `Qt::Tool` window that samples the area around the cursor and paints it with a grid |
| `ColorPickerWidget` | Owns the UI, drives the pick session, emits `CustomMouseClicked` |

`ColorPickerWidget` and `ColorGrabber` never reference each other. They are connected in `main.cpp`:

```
ColorPickerWidget::CustomMouseClicked  ->  ColorGrabber::GrabColor
ColorGrabber::ColorGrabbed             ->  ColorPickerWidget::SetColor
```

## Notes

**Cursor position is polled, the click is not.** A pick session runs a ~60 Hz timer that reads `QCursor::pos()`, which is a real global query and works regardless of which window the pointer is over. `mouseMoveEvent()` is unusable here because it only fires while the pointer is inside the application's own windows.

**The click is caught two ways, on purpose.** `grabMouse()` routes clicks from anywhere on the screen to the picker, which is also what stops the click from reaching the application underneath. But `SetCapture` on Windows only holds while the application is the foreground window, and a mouse grab is not guaranteed on every platform, so the poll also reads the global button state directly as a fallback. Both paths funnel into one method that clears the session flag first, so a pick can never fire twice.

**`QGuiApplication::mouseButtons()` is not a global query.** It reports the button state as of the last mouse event *delivered to this application*. While the cursor is over another program no events arrive, that state never changes, and clicks outside the window are silently missed. On Windows the code uses `GetAsyncKeyState()` instead, honouring the "swap mouse buttons" accessibility setting through `SM_SWAPBUTTON`. On other platforms it falls back to the Qt state and therefore depends entirely on the mouse grab holding.

**Wayland will not work.** Compositors deny both screen capture and mouse grabbing to ordinary clients; a port needs the `xdg-desktop-portal` screenshot interface. X11 is untested but has no known blocker.

**While a pick is active, the Start Grabbing button cannot be clicked again.** The mouse grab consumes that click and treats it as a pick at that position. `Esc` is the way out.

**The magnifier does not currently magnify.** `kSample` equals `kDiameter`, so the sampled area is drawn 1:1 and the 9×9 grid is decorative rather than one cell per screen pixel. Setting `kSample = kCells` in `MagnifierWidget.h` turns it into a real pixel grid, and as a side effect fixes the magnifier window partially sampling itself — at 1:1 its 160 px circle, offset by 20 px from the cursor, overlaps the area it is capturing.

## Roadmap

- [ ] Real magnification with one grid cell per screen pixel
- [ ] Keyboard nudging of the cursor by one pixel for precise picks
- [ ] Copy the picked colour to the clipboard in a chosen notation
- [ ] History of recent picks
- [ ] Global hotkey to start a pick without focusing the window
- [ ] Wayland support through `xdg-desktop-portal`
- [ ] Unit tests for the magnifier's centring maths, which is the part most likely to regress
