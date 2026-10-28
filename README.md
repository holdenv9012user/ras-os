# ras-os

ras-os is a custom operating system project in its initial setup stage.

## Project Status

This repository currently contains project documentation and folders reserved for
release images. Operating system source code, build tools, and bootable releases
have not been added yet.

## Repository Structure

```text
ras-os/
├── releases/
│   ├── iso/       # ISO release images
│   └── img/       # IMG release images
├── AGENTS.md      # Contributor guidelines
├── LICENSE        # MIT license
└── README.md
```

## Releases

Release images will be stored by file format:

- [ISO images](releases/iso/) — `.iso` files.
- [IMG images](releases/img/) — `.img` files.

The release folders currently contain only placeholders. Keep a placeholder in
each folder until a release image is added; Git does not track empty folders.

Installation instructions and supported hardware will be documented when bootable
releases are available.

## Development & Contributing

Read [Repository Guidelines](AGENTS.md) before contributing. Build and test
commands will be documented here when the development toolchain is established.

## License

ras-os is licensed under the [MIT License](LICENSE).

Copyright (c) 2026 ras-os contributors.
