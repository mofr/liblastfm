/***************************************************************************
 *   Copyright 2005-2009 Last.fm Ltd.                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/

#include <lastfm/global.h>
#include <QDomDocument>
#include <QDomElement>

namespace lastfm
{
    /** Qt's XmlQuery implementation is totally unimpressive, so this is a 
      * hack that feels like jQuery */
    class LASTFM_DLLEXPORT XmlQuery
    {
        QDomDocument domdoc;
        QDomElement e;

    public:
        /** we assume the bytearray is an XML document, this object will then
          * represent the documentElement of that document, eg. if this is a
          * Last.fm webservice response:
          *
          * XmlQuery xq = lastfm::ws::parse(response);
          * qDebug() << xq["artist"].text()
          *
          * Notice the lfm node is not referenced, that is because it is the
          * document-element of the XML document.
          */
        XmlQuery( const QByteArray& );
        
        XmlQuery( const QDomElement& e, const char* name = "" ) : e( e )
        {
            if (e.isNull()) qWarning() << "Expected node absent:" << name;
        }

        /** Selects a DIRECT child element, you can specify attributes like so:
          *
          * e["element"]["element attribute=value"].text();
          */
        XmlQuery operator[]( const QString& name ) const;
        QString text() const { return e.text(); }
        
        /** selects all children with specified name, recursively */
        QList<XmlQuery> children( const QString& named ) const;

        operator QDomElement() const { return e; }
    };
}


inline QDebug operator<<( QDebug d, const lastfm::XmlQuery& xq )
{
    QString s;
    QTextStream t( &s, QIODevice::WriteOnly );
    QDomElement(xq).save( t, 2 );
    return d << s;
}
