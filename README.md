# Soft_Renderer

从零开始写一个软光栅渲染器

## Built with

使用glfw创建窗口，画面用imgui进行显示

- glfw

- glm

- imgui

- stb_image.h


## Feature

- Bresenham描线算法
- Edge-Walking三角形填充算法
- 屏幕空间背面剔除
- 用了z缓冲做深度测试
- 纹理映射：邻近纹理采样，双线性纹理采样
- phong模型
- 不同的照明光源：定向光源，点光源，聚光灯

## Some Rendering pictures

#### triangle:

![img](https://cdn.nlark.com/yuque/0/2024/png/35466668/1712820532696-29c659f8-4fea-4eef-8b1e-afcc57599000.png)

#### box:

![img](https://cdn.nlark.com/yuque/0/2024/png/35466668/1712898664614-d6ea9ae4-024c-4c8b-9c59-d44068f462a9.png)

#### model:

![img](https://cdn.nlark.com/yuque/0/2024/png/35466668/1713877481762-37faabef-8a62-40ae-9d44-ada3992dd38b.png)

## Attention:

cmake(win):

```
cmake .. -DCMAKE_GENERATOR="Unix Makefiles"
```

u can:

1. mkdir build
2. cd build
3. cmake ..
4. make
5. ./demo

## 后续可做（目前想法）：

1. 窗口画面调整时渲染图像不变(窗口全屏化)
2. 多线程加速



