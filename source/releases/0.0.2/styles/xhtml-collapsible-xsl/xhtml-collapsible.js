/*
 * Toggles the visibility of the element identified by 'id'.
 */
function toggle( id, base )
{
  var ls = document.getElementById( id + "_ls" );
  var cl = document.getElementById( id + "_cl" );

  if (ls) {
    if (ls.style.display) {
      ls.style.display = "";
      if (cl) {
        cl.alt = "-";
        cl.src = base + "/minus.png";
      }
    }
    else {
      ls.style.display = "none";
      if (cl) {
        cl.alt = "+";
        cl.src = base + "/plus.png";
      }
    }
  }
}

/* vim: set sw=2 et: */
