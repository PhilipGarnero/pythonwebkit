import gtk
import pywebkitgtk

url = "http://www.gnu.org/software/pythonwebkit"
wv = pywebkitgtk.WebView(1024,768, url=url)

def _doc_loaded(*args):
    doc = wv.GetDomDocument()
    txt = doc.createTextNode("hello")
    doc.body.appendChild(txt)

wv.SetDocumentLoadedCallback(_doc_loaded)
gtk.main()

