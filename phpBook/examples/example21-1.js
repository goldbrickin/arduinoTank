function O(obj)
{
  if (typeof obj == 'object') return obj
  else return document.getElementById(obj)
}
