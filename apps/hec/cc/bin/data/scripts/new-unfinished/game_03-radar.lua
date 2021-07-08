Utils = require "Utils"

box2d = require "LuaBox2D"
uber = UberObj("Kinect")

game_time = 0



DraggableVertex = class()

function DraggableVertex:__init(x, y)
	self.x = x
	self.y = y
	self.bBeingDragged = false
	self.bOver = false
	self.radius = 4
end

-- array of DraggableVertex objects using a lua table
curveVertices = {
	DraggableVertex(326, 209),
	DraggableVertex(306, 279),
	DraggableVertex(265, 331),
	DraggableVertex(304, 383),
	DraggableVertex(374, 383),
	DraggableVertex(418, 209),
	DraggableVertex(345, 279)
}

-- number of vertexes in the array
nCurveVertexes = 7

function setup()
	
	of.clear(0, 0, 0, 0)

	of.setFrameRate(60)
	of.setWindowTitle("projektor")
	of.setWindowPosition(1920 , 10) -- (1920 + 1680,0)

end

----------------------------------------------------

function update()
	
	uber.getFBO(fbo)
	totoroTex:setAlphaMask( fbo:getTexture() )

end

function mouseDragged(x, y, button)
	for i=1,nCurveVertexes do
		if curveVertices[i].bBeingDragged == true then
			curveVertices[i].x = x
			curveVertices[i].y = y
		end
	end
end

----------------------------------------------------
function mousePressed(x, y, button)
	for i=1,nCurveVertexes do
		local diffx = x - curveVertices[i].x
		local diffy = y - curveVertices[i].y
		local dist = math.sqrt(diffx*diffx + diffy*diffy)
		if dist < curveVertices[i].radius then
			curveVertices[i].bBeingDragged = true
		else
			curveVertices[i].bBeingDragged = false
		end	
	end
end

----------------------------------------------------
function mouseReleased(x, y, button)
	for i=1,nCurveVertexes do
		curveVertices[i].bBeingDragged = false	
	end
end


function keyPressed(key)
end

----------------------------------------------------
function draw()
	of.pushMatrix()
	of.pushStyle()
	
	of.scale(0.01, 0.01, 0.01)
	of.enableBlendMode(of.BLENDMODE_ADD)

	totoro:draw(0,0,1024,768)
	
	of.popStyle()
	of.popMatrix()
end

----------------------------------------------------
function exit()
	print("script finished")
end
