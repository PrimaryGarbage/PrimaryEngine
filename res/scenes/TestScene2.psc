type=Node
name=TestScene2
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
imagePath=./res/textures/abstract_stairs.png
children_start
children_end

type=Sprite
name=TestSprite1
position=0.000000,0.000000
rotation=0.000000
scale=1.000000,1.000000
pivot=0.500000,0.500000
width=100.000000
height=100.000000
zIndex=0.000000
imagePath=./res/textures/TestTexture.png
children_start
children_end

type=Sprite
name=TestSprite2
position=5.000000,-137.000000
rotation=5.513000
scale=1.000000,1.000000
pivot=0.500000,0.500000
width=100.000000
height=100.000000
zIndex=0.000000
imagePath=./res/textures/TestTexture.png
children_start
children_end

type=ActorCamera2D
name=TestActorCamera
position=-29.813408,-172.889069
rotation=5.513000
scale=1.000000,1.000000
pivot=0.500000,0.500000
zNear=-1.000000
zFar=1.000000
zoom=1.000000
targetPath=/TestScene2/TestSprite2
initialOffset=0.000000,-50.000000
stiffness=0.070000
rotateWithTarget=1
children_start
children_end

children_end
