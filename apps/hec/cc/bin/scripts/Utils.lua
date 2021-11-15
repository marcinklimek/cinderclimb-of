local Utils = {max_fps = 60}


-- text ------

local franklinBook = of.TrueTypeFont()
franklinBook:load("fonts/frabk.ttf", 16)

local franklinBookBig = of.TrueTypeFont()
franklinBookBig:load("fonts/frabk.ttf", 32)

function Utils.drawTextCentered(str, x, y, big)

	of.pushMatrix()
	of.translate(x, y)
	of.scale(0.01, 0.01, 0.01)
	of.setHexColor(0xFFFFFF)

	if big == true then
		local rect = franklinBookBig:getStringBoundingBox(str, 0, 0);
		-- this is the actual midpt (x + w/2, y + h/2);
		local centerx = rect.x + rect.width / 2
		local centery = rect.y + rect.height / 2		
		franklinBookBig:drawString( str, -centerx, -centery)
	else
		local rect = franklinBookBig:getStringBoundingBox(str, 0, 0);
		-- this is the actual midpt (x + w/2, y + h/2);
		local centerx = rect.x + rect.width / 2
		local centery = rect.y + rect.height / 2		
		franklinBook:drawString( str, -centerx, -centery)
	end

	of.popMatrix()
end

function Utils.drawText(str, x, y, big)

	if big == true then
		franklinBookBig:drawString(str, x, y)
	else
		franklinBook:drawString(str, x, y)
	end

end


-- table ------

function Utils.table_length(T)
	local count = 0
	
	for _ in pairs(T) do 
		count = count + 1 
	end
	
	return count
end



return Utils