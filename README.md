# cg-vault
Experiments with 3D graphics using OpenGL.
Part of the coursework of discipline IA725 - Computer Graphics 1 (2021-s2).
https://www.dca.fee.unicamp.br/cursos/IA725/2s2021/

Features:
- Patch triangulation, used to generate sphere and torus mesh;
- Bézier surface for Teapot mesh creation;
- Perspective and orthogonal camera projection;
- RGB - HSV conversion;
- Simple object texturing;
- Phong shading;
- Simple UI to control scene rendering parameters;
- Shadow mapping.

## Build instructions

```
git clone --recurse-submodules https://github.com/matorsoni/cg-vault.git
cd cg-vault
mkdir build
cd build
cmake ..
make
./demo
```

## A few samples...

- Final scene rendering

![Screenshot from 2023-09-25 21-32-56](https://github.com/matorsoni/cg-vault/assets/33296520/6c743482-764d-4a02-8a43-a062c1661fc4)


- Commit 339f049

![Screenshot from 2021-10-22 13-28-45](https://user-images.githubusercontent.com/33296520/138490559-30fbf807-5bbd-49a4-b788-945ef2d56bd8.png)

- Commit 480d024

![Screenshot from 2021-09-02 23-22-17](https://user-images.githubusercontent.com/33296520/131940984-716edb90-8d0f-4772-b0e1-7befc3fa82b9.png)
