/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SOCKET_DATAGRAM_BRIDGE_H_
#define _VPR_SOCKET_DATAGRAM_BRIDGE_H_
// NOTE: this is the bridge class for use with SocketDatagram.h

#include <vpr/vprConfig.h>

#include <vpr/IO/Socket/Socket_t.h>


namespace vpr {

/**
 * Datagram socket interface.
 *
 * @author Patrick Hartling
 * @author Allen Bierbaum
 * @author Kevin Meinert
 */
template<class RealSocketDatagramImpl, class RealSocketDatagramImplParent>
class SocketDatagram_t : public Socket_t<RealSocketDatagramImplParent> {
public:
    /**
     * Default constructor.
     */
    SocketDatagram_t (void)
        : m_socket_dgram_imp()
    {
        m_socket_imp = &m_socket_dgram_imp;
    }

    /**
     * Constructor.  This takes a reference to a vpr::InetAddr object giving
     * the local socket address and a reference to a vpr::InetAddr object
     * giving the remote address.
     *
     * @pre addr is a reference to a valid vpr::InetAddr object.
     * @post A socket is created using the contents of addr.
     *
     * @param addr A reference to a vpr::InetAddr object.
     */
    SocketDatagram_t (const vpr::InetAddr& local_addr,
                      const vpr::InetAddr& remote_addr)
        : Socket_t<RealSocketDatagramImplParent>(),
          m_socket_dgram_imp(local_addr, remote_addr)
    {
        m_socket_imp = &m_socket_dgram_imp;
    }

    /**
     * Copy constructor.
     *
     * @param sock The source socket object to be copied.
     */
    SocketDatagram_t (const SocketDatagram_t& sock)
        : m_socket_dgram_imp(sock.m_socket_dgram_imp)
    {
        m_socket_imp = &m_socket_dgram_imp;
    }

    /**
     * Destructor.  This currently does nothing.
     *
     * @pre None.
     * @post None.
     */
    virtual ~SocketDatagram_t (void) {
        /* Do nothing. */ ;
    }

    /**
     * Receives a message from the designated source.
     */
    inline Status
    recvfrom (void* msg, const size_t len, const int flags,
              vpr::InetAddr& from, ssize_t& bytes_read,
              const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return m_socket_dgram_imp.recvfrom(msg, len, flags, from, bytes_read,
                                           timeout);
    }

    /**
     * Receives a message from the designated source.
     */
    Status
    recvfrom (std::string& msg, const size_t len, const int flags,
              vpr::InetAddr& from, ssize_t& bytes_read,
              const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        msg.resize(length);
        memset(&msg[0], '\0', msg.size());

        return recvfrom((void*) &msg[0], msg.size(), flags, from, bytes_read,
                        timeout);
    }

    /**
     * Receives a message from the designated source.
     */
    Status
    recvfrom (std::vector<vpr::Uint8>& msg, const size_t len, const int flags,
              vpr::InetAddr& from, ssize_t& bytes_read,
              const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        Status retval;

        msg.resize(length);

        memset(&msg[0], '\0', msg.size());
        retval = recvfrom((void*) &msg[0], msg.size(), flags, from, bytes_read,
                          timeout);

        // Size it down if needed, if (bytes_read==length), then resize does
        // nothing.
        if ( bytes_read >= 0 ) {
            msg.resize(bytes_read);
        }

        return retval;
    }

    /**
     * Sends a message to the designated recipient.
     */
    inline Status
    sendto (const void* msg, const size_t len, const int flags,
            const vpr::InetAddr& to, ssize_t& bytes_sent,
            const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        return m_socket_dgram_imp.sendto(msg, len, flags, to, bytes_sent,
                                         timeout);
    }

    /**
     * Sends a message to the designated recipient.
     */
    inline Status
    sendto (const std::string& msg, const size_t len, const int flags,
            const vpr::InetAddr& to, ssize_t& bytes_sent,
            const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        vprASSERT(length <= msg.size() && "Length is bigger than data given");
        return sendto(msg.c_str(), length, flags, to, bytes_sent, timeout);
    }

    /**
     * Sends a message to the designated recipient.
     */
    inline Status
    sendto (const std::vector<vpr::Uint8>& msg, const size_t len,
            const int flags, const vpr::InetAddr& to, ssize_t& bytes_sent,
            const vpr::Interval timeout = vpr::Interval::NoTimeout)
    {
        vprASSERT(length <= msg.size() && "Length is bigger than data given");
        return sendto((const void*) &msg[0], length, flags, to, bytes_sent,
                      timeout);
    }

    /**
     * Gets the multicast interface for this datagram socket.
     */
    inline bool
    getMcastInterface (vpr::InetAddr& mcast_if) {
        return m_socket_imp->getMcastInterface(mcast_if);
    }

    /**
     * Sets the multicast interface for this datagram socket.
     */
    inline bool
    setMcastInterface (const vpr::InetAddr& mcast_if) {
        return m_socket_imp->setMcastInterface(mcast_if);
    }

    /**
     * Gets the multicast time-to-live parameter for packets sent on this
     * socket.
     */
    inline bool
    getMcastTimeToLive (vpr::Uint8& ttl) {
        return m_socket_imp->getMcastTimeToLive(ttl);
    }

    /**
     * Sets the multicast time-to-live parameter for packets sent on this
     * socket.
     */
    inline bool
    setMcastTimeToLive (const vpr::Uint8 ttl) {
        return m_socket_imp->setMcastTimeToLive(ttl);
    }

    /**
     *
     */
    inline bool
    getMcastLoopback (vpr::Uint8& loop) {
        return m_socket_imp->getMcastLoopback(loop);
    }

    /**
     *
     */
    inline bool
    setMcastLoopback (const vpr::Uint8 loop) {
        return m_socket_imp->setMcastLoopback(loop);
    }

    /**
     *
     */
    inline bool
    addMcastMember (const vpr::McastReq& request) {
        return m_socket_imp->addMcastMember(request);
    }

    /**
     *
     */
    inline bool
    dropMcastMember (const vpr::McastReq& request) {
        return m_socket_imp->dropMcastMember(request);
    }

protected:
    /// Platform-specific datagram socket implementation object
    RealSocketDatagramImpl m_socket_dgram_imp;
};

}; // End of vpr namespace


#endif	/* _VJ_SOCKET_DATAGRAM_BRIDGE_H_ */
