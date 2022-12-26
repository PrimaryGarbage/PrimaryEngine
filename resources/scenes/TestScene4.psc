type=Node
name=TestScene4
children_start
type=Sprite
name=TestBackground
position=0.000000,0.000000
rotation=0.000000
scale=1.000000,1.000000
pivot=0.000000,0.000000
width=1920.000000
height=1080.000000
zIndex=0.000000
imagePath=D:\Projects\C++\PrimaryEngine\out\bin\.\resources\textures\abstract_stairs.png
children_start
children_end

type=Sprite
name=TestSprite1
position=952.000000,1335.000000
rotation=0.000000
scale=1.000000,1.000000
pivot=0.500000,0.500000
width=100.000000
height=100.000000
zIndex=0.000000
imagePath=D:\Projects\C++\PrimaryEngine\out\bin\.\resources\textures\TestTexture.png
children_start
children_end

type=Sprite
name=TestSprite2
position=200.000000,200.000000
rotation=0.000000
scale=1.000000,1.000000
pivot=0.500000,0.500000
width=100.000000
height=100.000000
zIndex=0.000000
imagePath=D:\Projects\C++\PrimaryEngine\out\bin\.\resources\textures\TestTexture.png
children_start
children_end

type=ActorCamera2D
name=TestActorCamera
position=951.999817,1334.992798
rotation=0.000000
scale=1.000000,1.000000
pivot=0.500000,0.500000
zNear=-1.000000
zFar=1.000000
zoom=1.000000
targetPath=/TestSprite1
initialOffset=0.000000,0.000000
stiffness=0.010000
rotateWithTarget=1
children_start
children_end

type=Label
name=Label
position=123.000000,-120.000000
rotation=0.000000
size=100.000000,100.000000
pivot=0.000000,0.000000
anchor=0.000000,1.000000
text=Imagine this is UI
textColor=0.892157,0.885983,0.822184,1.000000
children_start
type=Label
name=ChildLabel
position=199.000000,-76.000000
rotation=0.000000
size=65.000000,65.000000
pivot=0.000000,0.000000
anchor=0.000000,0.000000
text=And this is child UI
textColor=0.230680,0.980392,0.000000,1.000000
children_start
children_end

children_end

children_end
