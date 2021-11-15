local ub = UberObj("Kinect")

verdana = of.TrueTypeFont()

local box2d = require 'LuaBox2D'
local Vec2 = box2d.Vec2

local radius = 60

local gravity = Vec2(0, 10)
world = box2d.World(gravity)

local bodyDef = box2d.BodyDef()
bodyDef.type = 'static'
bodyDef.position = Vec2(500, 700)
bodyDef.gravityScale = 0.0
local groundBody = world.createBody(bodyDef)
local groundBox = box2d.PolygonShape()
groundBox.setAsBox(500, 50)
local groundFixture = groundBody.createFixture(groundBox, 0)

local body

function createBody()

	local bodyDef = box2d.BodyDef()
	bodyDef.type = 'dynamic'
	bodyDef.position = Vec2(500, 500)
	body = world.createBody(bodyDef)
	local dynamicBox = box2d.PolygonShape()
	dynamicBox.setAsBox(30, 30)
	local fixtureDef = box2d.FixtureDef()
	fixtureDef.shape = dynamicBox
	fixtureDef.density = 1
	fixtureDef.friction = 0.3
	fixtureDef.gravityScale = 10000
	fixtureDef.restitution = 0.5
	local dynamicFixture = body.createFixture(fixtureDef)
end


local timeStep = 1
local velocityIterations = 8
local positionIterations = 3

local counter = 0



function checkContact()
	
	
	local contact = world.contact

	while contact do
		counter = counter + 1	
		contact.fixtureB.body.setTransform( Vec2(5, 000)*100, 0 )
		
		--createBody()

		contact = contact.next
	end

end



-- world.setContactListener(contactL)



-- _CRT_SECURE_NO_WARNINGS
----------------------------------------------------
function setup()
	print("SETUP\n")
	print( ("%4.2f %4.2f\n"):format(groundBody.position.x, groundBody.position.y) )

	verdana:load("fonts/verdana.ttf", 8, false, true)
	verdana:setLineHeight(20)	

	createBody()
end


player = {}

----------------------------------------------------
function update()
	world.step(timeStep, velocityIterations, positionIterations)
	world.clearForces()

	for i=1, ub.numJoints do
		local vtx = ub.joint(i)
		
	end

	checkContact()	
end

----------------------------------------------------
function draw()
	of.setColor(255, 130, 0)
	of.fill()
	of.drawCircle(body.position.x, body.position.y, radius)

	of.setPolyMode(of.POLY_WINDING_ODD)
	of.setColor(255, 0, 0)
	of.beginShape()
		
	local n = groundBox.vertexCount

	for i=1,n do
		
		local vtx = groundBox.vertex(i) + groundBody.position
		of.vertex(vtx.x, vtx.y)
		
	end
		
	of.endShape(true)	


	of.setColor(0, 130, 0)
	of.fill()

	for i=1, ub.numJoints do
		local vtx = ub.joint(i)
		of.drawCircle(vtx.x, vtx.y, 10)
	end


	verdana:drawString( ("%i"):format(ub.numJoints), 10, 150)
	verdana:drawString( ("%i"):format(counter), 10, 210)
	verdana:drawString( ("%i"):format(ub.numJoints), 10, 270)
end

----------------------------------------------------
function exit()
	print("box2d script finished")
end