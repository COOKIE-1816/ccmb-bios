Official BIOS for CCMB motherboard (cookie custom mother board, **not the CCMB google searches!!!**)

# Building from sources
1. Make sure you are using linux and your system statisfies build dependicences.  
2. Clone git repository and navigate to `ccmb-bios`.  
```shell
git clone https://github.com/COOKIE-1816/ccmb-bios.git
cd ccmb-bios 

# additionaly, you can download the zip file and extract its contents.
```
3. Run `buid.sh` script to build from sources.
```shell
bash ./build.sh
```

# Directory structure
| Parent directory | Directory | Description                                |
|------------------|-----------|--------------------------------------------|
|                  | `/`       | The root directory                         |
| `/`              | `built`   | Build directory                            |
| `/built`         | `asm`     | Assembly files generated by compiler       |
| `/built`         | `bin`     | Compiled binaries (output files)           |
| `/`              | `docs`    | Docs                                       |
| `/`              | `error`   | Source files for displaying error codes    |
| `/`              | `hardware`| Source code for controlling devices        |

