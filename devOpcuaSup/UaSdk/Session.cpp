/*************************************************************************\
* Copyright (c) 2018-2020 ITER Organization.
* This module is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/

/*
 *  Author: Ralph Lange <ralph.lange@gmx.de>
 *
 *  based on prototype work by Bernhard Kuner <bernhard.kuner@helmholtz-berlin.de>
 */

#include <iostream>

#include <uaplatformlayer.h>
#include <uabase.h>

#include <epicsThread.h>

#include "Session.h"
#include "SessionUaSdk.h"

#define st(s) #s
#define str(s) st(s)

namespace DevOpcua {

static epicsThreadOnceId opcuaUaSdk_once = EPICS_THREAD_ONCE_INIT;

static
void opcuaUaSdk_init (void *junk)
{
    (void)junk;
    UaPlatformLayer::init();
}

void
Session::createSession (const std::string &name, const std::string &url,
                        const int debuglevel, const bool autoconnect)
{
    epicsThreadOnce(&opcuaUaSdk_once, &opcuaUaSdk_init, nullptr);
    new SessionUaSdk(name, url, autoconnect, debuglevel);
}

Session &
Session::findSession (const std::string &name)
{
    return static_cast<Session &>(SessionUaSdk::findSession(name));
}

bool
Session::sessionExists (const std::string &name)
{
    return SessionUaSdk::sessionExists(name);
}

void
Session::showAll (const int level)
{
    SessionUaSdk::showAll(level);
}

void
Session::showOptionHelp ()
{
    std::cout << "Options:\n"
              << "clientcert   path to client certificate [none]\n"
              << "clientkey    path to client private key [none]\n"
              << "batch-nodes  max. nodes per service call [0 = no limit]"
              << std::endl;
}

const std::string &
opcuaGetDriverName ()
{
    static const std::string sdk("Unified Automation C++ Client SDK v"
                                 str(PROD_MAJOR) "." str(PROD_MINOR) "."
                                 str(PROD_PATCH) "-" str(PROD_BUILD));
    return sdk;
}

std::string Session::securityCertificateTrustListDir;
std::string Session::securityCertificateRevocationListDir;
std::string Session::securityIssuersCertificatesDir;
std::string Session::securityIssuersRevocationListDir;
std::string Session::securityClientCertificateFile;
std::string Session::securityClientPrivateKeyFile;

} // namespace DevOpcua
