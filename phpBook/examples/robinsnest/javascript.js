// Example 26-14: javascript.js

canvas               = O('logo')
context              = canvas.getContext('2d')
context.font         = 'bold italic 97px Georgia'
context.textBaseline = 'top'
image                = new Image()
image.src            = 'robin.gif'

image.onload = function()
{
  gradient = context.createLinearGradient(0, 0, 0, 89)
  gradient.addColorStop(0.00, '#faa')
  gradient.addColorStop(0.66, '#f00')
  context.fillStyle = gradient
  context.fillText(  "R  bin's Nest", 0, 0)
  context.strokeText("R  bin's Nest", 0, 0)
  context.drawImage(image, 64, 32)
}

function O(i)
{
  if (typeof i == 'object') return i
  else return document.getElementById(i)
}

function S(i)
{
  return O(i).style
}

function C(n)
{
  var t = document.getElementsByTagName('*')
  var o = []

  for (var i in t)
  {
    var e = typeof t[i] == 'object' ? t[i].className : ''
    
    if (e                        ==  n ||
        e.indexOf(' ' + n + ' ') != -1 ||
        e.indexOf(      n + ' ') ==  0 ||
        e.indexOf(' ' + n      ) == (e.length - n.length - 1))
          o.push(t[i])
  }

  return o
}
