# rasterize
First person shooter

**Warning: flashing lights**

# Controls
* WASD: movement
* Space: jump
* Mouse: rotation
* Left click: shoot
* Right click: toggle scope
* Q: restart
* 1: equip gun
* 2: equip knife


https://user-images.githubusercontent.com/73869536/173174856-fe682b1f-32b9-437f-b715-c89d67fd1122.mp4


# Building
Dependencies: sdl2, sdl2_ttf, sdl2_mixer

## Linux
```
git clone https://github.com/longwatermelon/rasterize
cd rasterize
make
./a.out
```

## MacOS
Requires [brew](https://brew.sh)

```
git clone https://github.com/longwatermelon/rasterize
cd rasterize
make INCLUDE=-I$(brew --prefix)/include LIBRARIES=-L$(brew --prefix)/lib FLAGS=-Wno-error=unused-command-line-argument
./a.out
```

## Windows

Good luck buddy
