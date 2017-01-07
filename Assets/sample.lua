mat1 = gr.material({0.54, 0.27, 0.07}, {0.2, 0.2, 0.2}, 25)
mat3 = gr.material({0.3, 0.5, 0.4}, {1, 1, 1}, 100)
mat4 = gr.material({0.7, 0.2, 0.4}, {0.8, 0.7, 0.6}, 100)
mat2 = gr.material({0.1, 0.1, 0.1}, {0.5, 0.4, 0.2}, 10)
mat5 = gr.material({0.9, 0.8, 0.9}, {1, 1, 1}, 1000)
mat6 = gr.material({0.5, 0.7, 0}, {0.2,0.8, 0.8}, 100)
mat7 = gr.material({1, 0, 0}, {0.2,0.4, 0.4}, 15)
scene_root = gr.node('root')

-- A small stellated dodecahedron.

steldodec = gr.mesh( 'boat', 'Assets/boat.obj' )
steldodec:set_material(mat1)
scene_root:add_child(steldodec)

s1 = gr.nh_sphere('s1', {-40, -130, -50}, 50)
scene_root:add_child(s1)
s1:set_material(mat2)

s2 = gr.nh_sphere('s2', {-40, -40, 20}, 50)
scene_root:add_child(s2)
s2:set_material(mat3)

s3 = gr.nh_sphere('s3', {-10, 20, -80}, 20)
scene_root:add_child(s3)
s3:set_material(mat4)

s4 = gr.nh_sphere('s4', {-70, -10, 170}, 20)
scene_root:add_child(s4)
s4:set_material(mat5)


s5 = gr.nh_sphere('s5', {0, -40, 170}, 10)
scene_root:add_child(s5)
s5:set_material(mat7)


steldodec = gr.mesh( 'dodec', 'Assets/smstdodeca.obj' )
steldodec:set_material(mat6)
scene_root:add_child(steldodec)

light1 = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
light2 = gr.light({400.0, 100.0, 150.0}, {0.6, 0.8, 0.7}, {1, 0, 0})
light3 = gr.light({300.0, 80.0, 150.0}, {0.4, 0.4, 0.7}, {1, 0, 0})

gr.render(scene_root, 'sample.png', 512, 512,
	  {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {light1, light2,light3})
