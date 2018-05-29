local Sprite = {}

local Utils = require "Utils"

function Sprite.Atlas(fps, cyclic)

	local self = {
		time = 0,
		index = 0,
		frames = 0,
		atlas = {},
		rate = fps/Utils.max_fps,
		is_cyclic = cyclic,
		is_end = false,
		start = false,
	}

	function self.add(imagePath)
		
		local img = of.Image()
		img:load(imagePath)
		table.insert(self.atlas, img)

		self.frames = self.frames + 1
	end

	function self.finished()
		return self.is_end
	end
	
	function self.play()
		self.start = true
		self.is_end = false
	end

	function self.update()

		if self.start == true then

			self.time = self.time + self.rate

			if self.time >= 1 then
				self.time = 0
				self.index = self.index + 1 

				if self.cyclic == true then
					self.index = self.index % self.frames
				else
					--print ("sprite end", self.index, self.frames, self.index >= self.frames)
					if self.index == self.frames then
						self.index = 0
						self.start = false
						self.is_end = true
					end
				end
			end
		end
	end

	function self.draw(x, y, w, h)

		if self.start == true then
			--print(self.index, (self.index % self.frames) + 1)
			self.atlas[(self.index % self.frames) + 1]:draw(x, y, w, h)
		end
	end

    return self
end


return Sprite