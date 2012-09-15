
function spi.GetPluginType()
	return spi.PluginType.Archive
end

function spi.GetPluginName()
	return "HSP DPM Plug-In - Lua Sample"
end

function spi.GetPluginFileTypes()
	return {
		{ filename = "*.dpm", description = "HSP DPM Archive" }
	}
end

function spi.IsSupported(filename, buf)
	return buf:sub(1, 4) == "DPMX"
end
