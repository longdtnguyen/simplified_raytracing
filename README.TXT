||==================================================================||
||																	||
||						CS 488 - Assignment 4						||
||							 Long Nguyen							||
||							  20455912								||
||																	||
||==================================================================||



//-------------------		COMPILATION			--------------------//

to compile run the following: in A4 folder
	- premake4 gmake
	- make

//-----------------------OBJECTIVE NOT COMPLETED --------------------//

1 ) NO shadow
2 ) NO hierachical model transformation

									
//------------------         Extra			------------------------//

1) Extra feature: Implemented Supersampling with Random Gamma for 16 rays per pixel

2) There is a picture of nonhier-bb.png to show the bounding for Mesh

3) There is a picture for the Custom Scene (NOTE: AGAIN no hierachical) : sample.png
   The Lua file is in Assets with the same name.

//---------------
NOTE: 
for nonhier.lua and hier.lua, I have changed the Path for it to get the Relative Path with Assets folder.
(Same applied with all .lua file)

From : steldodec = gr.mesh( 'dodec', 'smstdodeca.obj' )
To: steldodec = gr.mesh( 'dodec', 'Assets/smstdodeca.obj' )


- There is a new Mesh called boat.obj
downloading from:
http://www.models-resource.com/gamecube/legendofzeldathewindwaker/

