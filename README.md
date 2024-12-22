# Build On Linux

目前只考虑 StaticModel

```shell shell
sudo apt-get install build-essential mesa-utils libassimp-dev liblodepng-dev libgl1-mesa-dev libglew-dev libglfw3-dev  libglm-dev
mkdir build
cd build
cmake ..
make
./test
```


# OpenGL-MMD
Load pmx model and generate demo

- PMXLoader: Utilize Assimp for PMX model loading
- StaticModel: Implement texture-based PMX model rendering
- LightModel: Implement PMX model rendering with integrated lighting effects
- CompleteModel: Implement PMX model rendering with integrated lighting effects, background scenery, and VMD animations

![MMD Demo Screenshot](./images/Scene.png)