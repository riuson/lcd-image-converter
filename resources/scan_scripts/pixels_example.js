// 'argb' - pixel's data of type uint32

var a = (argb >> 24) & 0xff;
var r = (argb >> 16) & 0xff;
var g = (argb >> 8) & 0xff;
var b = (argb >> 0) & 0xff;

// place modified value back to variable 'argb'
argb = (a  << 24) | (r << 16) | (g << 8) | b;
