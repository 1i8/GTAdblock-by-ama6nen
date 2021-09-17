# Growtopia Ad blocker
Growtopia Ad blocker for disabling ads in weather background (billboards, airplanes) and in ad blocks.

## Alternative repo
https://github.com/selenmunu/GTAdblock

## How to use/install
1. Download release [here](https://github.com/ama6nen/GTAdblock/releases/tag/V1)
2. Replace existing SecureEngineSDK64.dll in Growtopia directory with the downloaded one
3. Done

## Features
* Disables airplanes and billboards in the default sunny weather background in all worlds
* Hide ads in actual ad blocks (todo)


## Notes
* SecureEngineSDK64 is an unused dll that does not do anything anymore, containing only empty functions. By naming our dll as this, gt will load it automatically at startup.
* This ad blocker is essentially a very tiny internal cheat. It is compatible with other internal cheats, because it does not hook anything.
