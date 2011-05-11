import sys
import webkitdfb

class WebView:
    def __init__(self, url):
        self.v = webkitdfb.WebView(1024,768,
                        url=url)
        self.is_loaded = False
        self.v.SetDocumentLoadedCallback(self._doc_loaded)

    def _doc_loaded(self):
        print "loaded callback called"
        self.is_loaded = True

if __name__ == '__main__':
    url = "http://www.gnu.org/software/pythonwebkit"
    if len(sys.argv) == 2:
        url = sys.argv[1]
    wv = WebView(url)

    while not wv.is_loaded:
        webkitdfb.loop()

    d = wv.v.GetDomDocument()
    b = d.body
    txt = d.createTextNode("hello")
    b.appendChild(txt)

    while 1:
        webkitdfb.loop()

