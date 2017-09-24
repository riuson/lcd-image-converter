var a = (argb_uint32 >> 24) & 0xff;
var r = (argb_uint32 >> 16) & 0xff;
var g = (argb_uint32 >> 8) & 0xff;
var b = (argb_uint32 >> 0) & 0xff;
argb_uint32 = (a  << 24) | (r << 16) | (g << 8) | b;
