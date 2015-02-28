/******************************************************************************
Magic XML by Tom Davies
-----------------------
Magically implements cross-browser code from w3schools.com/xsl/xsl_client.asp 
to pull in XML data and apply a specified XSLT on marked up elements.

More details at: http://www.t-davies.com/magicxml/

******************************************************************************/

/*

The MIT License (MIT)

Copyright (C) 2013 - Tom Davies (tom@t-davies.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/
+function(g,h,c){var e=0,f,a,n={xmlSourceAttribute:"data-xml",xslSourceAttribute:"data-xslt",xslParamAttribute:"data-xsl-params"};function d(j){var o=(g.ActiveXObject||"ActiveXObject" in g)?new ActiveXObject("Msxml2.XMLHTTP.3.0"):new XMLHttpRequest();o.open("GET",j,false);o.send();return o.responseXML}function b(o){if(g.ActiveXObject||"ActiveXObject" in g){var j=new ActiveXObject("MSXML2.FreeThreadedDOMDocument.6.0");j.async=false;j.load(o);return j}return d(o)}function k(j,p,r){var o=0,s,q=new XSLTProcessor();q.importStylesheet(p);if(r!==c){for(o;o<r.length;o++){s=r[o];q.setParameter((s.xmlns!==c)?s.xmlns:null,s.name,s.value)}}return q.transformToFragment(j,h)}function m(j,p,s){var o=0,r,q=new ActiveXObject("MSXML2.XSLTemplate.6.0");q.stylesheet=p;r=q.createProcessor();r.input=j;if(s!==c){for(o;o<s.length;o++){parameter=s[o];r.addParameter(parameter.name,parameter.value,parameter.xmlns)}}r.transform();return r.output}function l(j){return"["+j+"]"}var i={configure:function(o,j,p){if(typeof o==="string"){n.xmlSourceAttribute=o}if(typeof j==="string"){n.xslSourceAttribute=j}if(typeof p==="string"){n.xslParamAttribute=p}},transform:function(j,r,q){var o=d(j),p=b(r);if(g.ActiveXObject||"ActiveXObject" in g){return m(o,p,q)}else{return k(o,p,q)}},transformAndReplace:function(q,j,r,p){var o=i.transform(j,r,p);if(typeof q==="string"){q=h.querySelector(q)}if(g.ActiveXObject||"ActiveXObject" in g){q.innerHTML=o}else{q.parentNode.replaceChild(o,q)}},parse:function(j){if(typeof j!=="string"){j=""}var o=h.querySelectorAll(j+"["+n.xmlSourceAttribute+"]["+n.xslSourceAttribute+"]");for(e;e<o.length;e++){a=c;f=o[e];if(f.attributes[n.xslParamAttribute]){a=JSON.parse(f.attributes[n.xslParamAttribute].value)}i.transformAndReplace(f,f.attributes[n.xmlSourceAttribute].value,f.attributes[n.xslSourceAttribute].value,a)}if(o.length===0){console.log("[Magic XML] No magic detected on page, is script loaded after all DOM elements?")}}};g.magicXML=i}(window,document);