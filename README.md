# Epic8 C Port

A basic C implementation of the Epic8 CPU Emulator with noticable differences such as the lack of debugging tools and a graphical display. Epic8C was designed for use with [SynnixOS](https://github.com/RaidTheWeb/SynnixOS) as a port


## Building

There are two different `make` build targets for Epic8C: `local` and `snx`

### Local Builds
`local` compiles the emulator with gcc with the standard linux libraries so you can run it as a normal C program

Run to build for local:
```
make local
```

### SynnixOS Builds
`snx` compiles the emulator using the SynnixOS cross compiler using it's libc, for use on the OS

Run to build for SynnixOS
