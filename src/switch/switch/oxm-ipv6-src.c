/*
 * Copyright (C) 2008-2013 NEC Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#include <stdint.h>
#include "trema.h"
#include "ofdp.h"
#include "oxm.h"


static uint32_t ipv6_src_field( const bool attr, const enum oxm_ofb_match_fields oxm_type );
static uint16_t ipv6_src_length( const match *match );
static uint16_t pack_ipv6_src( oxm_match_header *hdr, const match *match );


static struct oxm oxm_ipv6_src = {
  OFPXMT_OFB_IPV6_SRC,
  IPV6_ADDRLEN + sizeof( oxm_match_header ), 
  ipv6_src_field,
  ipv6_src_length,
  pack_ipv6_src
};


void 
init_oxm_ipv6_src( void ) {
  register_oxm( &oxm_ipv6_src );
}


static uint32_t
ipv6_src_field( const bool attr, const enum oxm_ofb_match_fields oxm_type ) {
  uint32_t field = 0;

  if ( attr && oxm_type == oxm_ipv6_src.type ) {
    field = OXM_OF_IPV6_SRC;
  }
  return field;
}


static uint16_t
ipv6_src_length( const match *match ) {
  uint16_t length = 0;
  
  if ( match->ipv6_src[ 0 ].valid ) {
    length = oxm_ipv6_src.length;
    if ( match->ipv6_src[ 0 ].mask != UINT8_MAX ) {
      length = ( uint16_t ) ( length * 2 );
    }
  }
  return length;
}


static uint16_t
pack_ipv6_src( oxm_match_header *hdr, const match *match ) {
  if ( match->ipv6_src[ 0 ].valid ) {
    *hdr = OXM_OF_IPV6_SRC;
    uint8_t *value = ( uint8_t * ) ( ( char * ) hdr + sizeof ( oxm_match_header ) );
    memcpy( value, &match->ipv6_src[ 0 ].value, IPV6_ADDRLEN );
    return oxm_ipv6_src.length;
  }
  return 0;
}


/*
 * Local variables:
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * End:
 */
