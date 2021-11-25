<p align="center">
  <img align="center" width="200" src="https://github.com/MemoOR/SmartWifiBlinds/blob/master/assets/README/logo_gdcode.png">
  <h1 align="center" style="margin: 0 auto 0 auto;">Smart Wifi Blinds</h1>
  <h5 align="center" style="margin: 0 auto 0 auto;">With ESP32 and Sinric Pro</h5>
</p>

<p align="center">
    <img src="https://img.shields.io/github/last-commit/MemoOR/SmartWifiBlinds">
    <img src="https://img.shields.io/github/issues/MemoOR/SmartWifiBlinds?label=issues">
    <img src="https://img.shields.io/github/stars/MemoOR/SmartWifiBlinds?color=purple&">
    <img alt="GitHub top language" src="https://img.shields.io/github/languages/top/MemoOR/SmartWifiBlinds?color=purple">
</p>

<p align="center">
  <img src="https://img.shields.io/github/languages/code-size/MemoOR/SmartWifiBlinds?color=purple">
  <img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/MemoOR/SmartWifiBlinds?color=purple">
  <img alt="Lines of code" src="https://img.shields.io/tokei/lines/github/MemoOR/SmartWifiBlinds?color=purple&label=total%20lines%20in%20repo">
  <img alt="GitHub language count" src="https://img.shields.io/github/languages/count/MemoOR/SmartWifiBlinds?color=purple">
</p>

# SmartWifiBlinds

## Smart Wifi Blinds, compatible with voice assistants.
### This specific project was created to move blinds that has more than one chain to open and close them (horizontal and vertical), like the ones on the next images:
<p align="center">
  <span align="left">
    <img width="330" src="https://github.com/MemoOR/SmartWifiBlinds/blob/master/assets/README/blinds_1.png">
  </span>

  <span align="right">
    <img width="300" height="395.47" src="https://github.com/MemoOR/SmartWifiBlinds/blob/master/assets/README/blinds_2.png">
  </span>
</p>

# Steps to make it work
## Go to Sinric Pro page, create an account and declare two devices of blinds type.
### Save your credentials and put them all in a file called credentials.h inside libs directory. It should look like this:

```
#define APP_KEY           "..."      // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
#define APP_SECRET        "..."   // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"
#define V_BLINDS_ID       "..."    // Should look like "5dc1564130xxxxxxxxxxxxxx"
#define H_BLINDS_ID       "..."    // Should look like "5dc1564130xxxxxxxxxxxxxx"
#define BAUD_RATE         115200                // Change baudrate to your need
```

## Flash your ESP-32 4MB with the downloaded code from this repository.

# Diagram connection
