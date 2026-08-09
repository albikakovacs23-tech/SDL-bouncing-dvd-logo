# Installation

## Requirements

You need:

* GCC
* Git
* SDL3 development files
* `pkg-config`

On Alpine Linux, install them with:

```bash
sudo apk add gcc git sdl3-dev pkgconf
```

## Clone the project

Clone the repository with:

```bash
git clone YOUR_GITHUB_REPOSITORY_URL
```

Then enter the project directory:

```bash
cd dvd-logo-sdl3
```

Make sure these files are present:

```text
dvd.c
dvd-logo.bmp
```

## Compile

Compile the program with:

```bash
gcc dvd.c -o dvd $(pkg-config --cflags --libs sdl3)
```

## Run

Start the program with:

```bash
./dvd
```

A window should appear with the DVD logo bouncing around the screen.

## Troubleshooting

### SDL3 cannot be found

If GCC reports that SDL3 cannot be found, make sure the development package is installed:

```bash
sudo apk add sdl3-dev
```

Then try compiling again.

### The image cannot be loaded

Make sure `dvd-logo.bmp` is in the **same directory** as the executable:

```text
dvd-logo-sdl3/
├── dvd
├── dvd.c
└── dvd-logo.bmp
```

Then run:

```bash
./dvd
```
