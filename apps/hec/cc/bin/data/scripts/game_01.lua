box2d = require 'LuaBox2D'

verdana = of.TrueTypeFont()

local function drawText(str, x, y)

	of.pushMatrix()
	of.translate(0, 0)
	of.scale(0.001, 0.001)

	verdana:drawString( str, 10, 150)

	of.popMatrix()
end


local function Ball(world, _x, _y, _r)
    
    -- public 
	local self = {
		name = "ball",
	    r = _r,
	    x = _x,
	    y = _y,
	}


	local bodyDef = box2d.BodyDef()
	bodyDef.position = box2d.Vec2(self.x, self.y)
	bodyDef.type = 'dynamic'

	local body = world.createBody(bodyDef)
	body.userData = self
	body.gravityScale = 1

	local circleShape = box2d.CircleShape()
	circleShape.radius = self.r
	
	local fixtureDef = box2d.FixtureDef()
	fixtureDef.shape = circleShape
	fixtureDef.density = 1
	fixtureDef.friction = 0.3
	fixtureDef.restitution = 0.2

	local fixture = body.createFixture(fixtureDef)

	function self.draw()
		of.setColor(255, 130, 0)
		of.fill()
		of.drawCircle(body.position.x, body.position.y, self.r)
	end


	function self.contact(obj)
		-- print("contact ", self.name, "->", obj.name)

		if obj.name == "ground" then

			body.setTransform( box2d.Vec2(self.x, self.y), 0 )
			body.linearVelocity = box2d.Vec2(0, 0)
			body.angularVelocity = 0
		end

		return true
	end

    return self
end


local function BallStatic(world, _x, _y, _r)
    
    -- public 
	local self = {
		name = "ballStatic",
	    r = _r,
	    x = _x,
	    y = _y,
	}

	local bodyDef = box2d.BodyDef()
	bodyDef.position = box2d.Vec2(self.x, self.y)
	bodyDef.type = 'dynamic'

	local body = world.createBody(bodyDef)
	body.userData = self
	body.gravityScale = 0

	local circleShape = box2d.CircleShape()
	circleShape.radius = self.r
	
	local fixtureDef = box2d.FixtureDef()
	fixtureDef.shape = circleShape
	fixtureDef.density = 1
	fixtureDef.friction = 1
	fixtureDef.restitution = 0.0
	--fixtureDef.sensor = true

	local fixture = body.createFixture(fixtureDef)

	function self.draw()
		-- of.setColor(255, 130, 0)
		-- of.fill()
		-- of.drawCircle(body.position.x, body.position.y, self.r)
	end


	function self.setPos(pos)
		self.x = pos.x
		self.y = pos.y

		body.setTransform( box2d.Vec2(pos.x, pos.y), 0 )
		body.linearVelocity = box2d.Vec2(0, 0)
		body.angularVelocity = 0		
	end

	function self.contact(obj)
		-- print("contact ", self.name, "->", obj.name)
	end

    return self
end


local function Scene()

	-- public
    local self = {
    	name = "ground",
    	balls = {}
	}

	-- private
	local timeStep = 1/50
	local velocityIterations = 8
	local positionIterations = 3

	self.balls = {}
	self.world = box2d.World(box2d.Vec2(0, 0.8), true)
	
	local bodyDef = box2d.BodyDef()
	bodyDef.type = 'static'
	bodyDef.position = box2d.Vec2(-100, 9)
	bodyDef.gravityScale = 0.0

	local groundBody = self.world.createBody(bodyDef)
	groundBody.userData = self

	local groundBox = box2d.PolygonShape()
	groundBox.setAsBox(200, 0.1)
	
	local groundFixture = groundBody.createFixture(groundBox, 0)




	local function checkContact()
		
		local contact = self.world.contact

		while contact do

			local obj = contact.fixtureA.body.userData
			if obj ~= nil then
				obj.contact(contact.fixtureB.body.userData)
			end

			local obj = contact.fixtureB.body.userData
			if obj ~= nil then
				obj.contact(contact.fixtureA.body.userData)
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

		for index, ball in ipairs(self.balls) do
			ball.draw()
		end		
	end

	function self.update()

		self.world.step(timeStep, velocityIterations, positionIterations)
		self.world.clearForces()

		checkContact()
	end

	function self.contact(obj)
		-- print("contact ", self.name, "->", obj.name)
	end


	function self.gameInit()
		self.balls = {}

		table.insert(self.balls, Ball(self.world, 0.30, -0.10, 0.05) )
		table.insert(self.balls, Ball(self.world, 0.90, -0.20, 0.10) )
		table.insert(self.balls, Ball(self.world, 1.30, -0.50, 0.15) )
		table.insert(self.balls, Ball(self.world, 3.00, -0.40, 0.20) )		
	end

	self.gameInit()

	return self
end



local function Player(world)

	-- public
    local self = {
    	name = "ground"
	}

	local uber = UberObj("Kinect")
	local joints = {}
	local points = {}

	table.insert(points, BallStatic(world, -1, -1, 0.3) )
	table.insert(points, BallStatic(world, -1, -2, 0.3) )
	table.insert(points, BallStatic(world, -1, -3, 0.3) )
	table.insert(points, BallStatic(world, -1, -4, 0.3) )
	table.insert(points, BallStatic(world, -1, -5, 0.3) )


	function self.update()

		joints = {}

		for i=1, uber.numJoints do
			
			local p = uber.joint(i)
			table.insert(joints,  p)

			points[i].setPos(p)
			
		end		

	end

	function self.draw()

		of.pushStyle()

		-- of.setColor(0, 130, 0)
		-- of.fill()
		-- for index, value in ipairs(joints) do
		-- 	of.drawCircle(value.x, value.y, 0.1)
		-- end

		for index, point in ipairs(points) do
			point.draw()
		end

		of.setHexColor(0x000000)
		of.drawBitmapString(("%i"):format(uber.numJoints), 0.10, 1.50)
		--drawText( ("%i"):format(uber.numJoints), 0.10, 1.50)

		of.popStyle()
	end

	return self
end


-- --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

function setup()
	print("SETUP\n")

	verdana:load("fonts/verdana.ttf", 8, false, true)
	verdana:setLineHeight(20)	

	scene = Scene()
	player = Player(scene.world)
end


----------------------------------------------------
function update()
	-- print("UPDATE\n")

	scene.update()
	player.update()

end

----------------------------------------------------
function draw()
	-- print("DRAW\n")

	scene.draw()
	player.draw()

end

----------------------------------------------------
function exit()
	print("box2d script finished")
end
