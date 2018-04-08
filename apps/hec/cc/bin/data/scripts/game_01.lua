box2d = require 'LuaBox2D'
uber = UberObj("Kinect")
verdana = of.TrueTypeFont()


local function Ball(world, x, y, r)
    
    -- public 
	local self = {
		name = "ball"
	}

	local radius = r

	local bodyDef = box2d.BodyDef()
	bodyDef.position = box2d.Vec2(x, y)
	bodyDef.type = 'dynamic'

	local body = world.createBody(bodyDef)
	body.userData = self
	
	local dynamicBox = box2d.PolygonShape()
	dynamicBox.setAsBox(radius, radius)
	
	local fixtureDef = box2d.FixtureDef()
	fixtureDef.shape = dynamicBox
	fixtureDef.density = 1
	fixtureDef.friction = 0.3
	fixtureDef.gravityScale = 1
	fixtureDef.restitution = 0.5

	local fixture = body.createFixture(fixtureDef)

	function self.draw()
		of.setColor(255, 130, 0)
		of.fill()
		of.drawCircle(body.position.x, body.position.y, radius)
	end

	function self.draw()
		of.setColor(255, 130, 0)
		of.fill()
		of.drawCircle(body.position.x, body.position.y, radius)
	end

	function self.contact()
		print("contact")
	end

    return self
end



local function Scene()

	-- public
    local self = {
    	name = "ground"
	}

	-- private
	local timeStep = 1
	local velocityIterations = 8
	local positionIterations = 3



	self.world = box2d.World(box2d.Vec2(0, 10))
	
	local bodyDef = box2d.BodyDef()
	bodyDef.type = 'static'
	bodyDef.position = box2d.Vec2(600, 500)
	bodyDef.gravityScale = 0.0

	local groundBody = self.world.createBody(bodyDef)
	-- groundBody.userData = self

	local groundBox = box2d.PolygonShape()
	groundBox.setAsBox(600, 10)
	
	local groundFixture = groundBody.createFixture(groundBox, 0)


	local function checkContact()
		
		local contact = self.world.contact

		while contact do

			local obj = contact.fixtureA.body.userData
			if obj ~= nil then
				obj:contact()
			end

			local obj = contact.fixtureB.body.userData
			if obj ~= nil then
				o:contact()
			end
			
			contact = contact.next
		end
	end

	function self.draw()

	    of.pushMatrix();
	    
	    of.translate(groundBody.position.x, groundBody.position.y);

		of.setPolyMode(of.POLY_WINDING_ODD)
		of.setColor(255, 0, 0)
		of.beginShape()
		
		local n = groundBox.vertexCount
		for i=1,n do
			local vtx = groundBox.vertex(i) 
			of.vertex(vtx.x, vtx.y)
		end
			
		of.endShape(true)	

		of.popMatrix()
	end

	function self.update()

		self.world.step(timeStep, velocityIterations, positionIterations)
		self.world.clearForces()

		checkContact()
	end

	return self
end

-- --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

function setup()
	print("SETUP\n")

	verdana:load("fonts/verdana.ttf", 8, false, true)
	verdana:setLineHeight(20)	

	scene = Scene()
	ball = Ball(scene.world, 0, 10, 20, 20)	
end


player = {}

----------------------------------------------------
function update()

	scene.update()

	player = {}
	for i=1, uber.numJoints do
		player.insert( uber.joint(i) )
	end

end

----------------------------------------------------
function draw()

	of.setColor(0, 130, 0)
	of.fill()
	for index, value in ipairs(player) do
		of.drawCircle(value.x, value.y, 10)
	end

	scene.draw()

	if ball ~= nil then
		ball.draw()
	end

	if ball2 ~= nil then
		ball2.draw()
	end

	verdana:drawString( ("%i"):format(uber.numJoints), 10, 150)
	verdana:drawString( ("%i"):format(uber.numJoints), 10, 270)
end

----------------------------------------------------
function exit()
	print("box2d script finished")
end
