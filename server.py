from twisted.internet.protocol import Factory, Protocol
from twisted.internet import reactor

import time
import _mysql
import config
import pprint

class SendContent(Protocol):
    def connectionMade(self):
        print 'Accepted connection'
    def dataReceived(self, data):
        date = time.time()
        values = data.split(',');
        pprint.pprint(values);
        if len(values) != 3:
            print 'Error: Bad Data'

        db = None;
        try:
            db = _mysql.connect(config.hostname, config.username, config.password, config.database)
            db.query("INSERT INTO tracker (date, speed, lat, lng)" +
                "VALUES ('" + str(date) + "', '" + str(values[0]) + "', '" + 
                str(values[1]) + "', '" + str(values[2]) + "')")
            print data
        except _mysql.Error, e:
            print "Error %d: %s" % (e.args[0], e.args[1])
        finally:
            if db: 
                db.close()

class SendContentFactory(Factory):
    protocol = SendContent

reactor.listenTCP(81, SendContentFactory())
reactor.run()
