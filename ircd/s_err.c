/*
 * IRC - Internet Relay Chat, ircd/s_err.c
 * Copyright (C) 1992 Darren Reed
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 1, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
/** @file
 * @brief Error handling support.
 * @version $Id: s_err.c 1905 2009-02-09 01:30:13Z entrope $
 */
#include "config.h"

#include "numeric.h"
#include "ircd_log.h"
#include "s_debug.h"

/* #include <assert.h> -- Now using assert in ircd_log.h */
#include <string.h>

/** Array of Numeric replies, indexed by numeric. */
static Numeric replyTable[] = {
/* 000 */
  { 0 },
/* 001 */
  { RPL_WELCOME, ":Welcome to the %s IRC Network%s%s, %s", "001" },
/* 002 */
  { RPL_YOURHOST, ":Your host is %s, running version %s", "002" },
/* 003 */
  { RPL_CREATED, ":This server was created %s", "003" },
/* 004 */
  { RPL_MYINFO, "%s %s %s %s %s", "004" },
/* 005 */
  { RPL_ISUPPORT, "%s :are supported by this server", "005" },
/* 006 */
  { 0 },
/* 007 */
  { 0 },
/* 008 */
  { RPL_SNOMASK, "%u :: Server notice mask (%#x)", "008" },
/* 009 */
  { 0 },
/* 010 */
  { 0 },
/* 011 */
  { 0 },
/* 012 */
  { 0 },
/* 013 */
  { 0 },
/* 014 */
  { 0 },
/* 015 */
  { RPL_MAP, ":%s%s%s %s [%u clients]", "015" },
/* 016 */
  { RPL_MAPMORE, ":%s%s --> *more*", "016" },
/* 017 */
  { RPL_MAPEND, ":End of /MAP", "017" },
/* 018 */
  { 0 },
/* 019 */
  { 0 },
/* 020 */
  { 0 },
/* 021 */
  { 0 },
/* 022 */
  { 0 },
/* 023 */
  { 0 },
/* 024 */
  { 0 },
/* 025 */
  { 0 },
/* 026 */
  { 0 },
/* 027 */
  { 0 },
/* 028 */
  { 0 },
/* 029 */
  { 0 },
/* 030 */
  { RPL_APASSWARN_SET, ":Channel Admin password (+A) set to '%s'.  Are you SURE you want to use this as Admin password? You will NOT be able to change this password anymore once the channel is more than 48 hours old!", "030" },
/* 031 */
  { RPL_APASSWARN_SECRET, ":Use \"/MODE %s -A %s\" to remove the password and then immediately set a new one.  IMPORTANT: YOU CANNOT RECOVER THIS PASSWORD, EVER; WRITE THE PASSWORD DOWN (don't store this rescue password on disk)! Now set the channel user password (+U).", "031" },
/* 032 */
  { RPL_APASSWARN_CLEAR, ":WARNING: You removed the channel Admin password (+A). If you disconnect or leave the channel without setting a new password then you will not be able to set it again!  SET A NEW PASSWORD NOW!", "032" },
/* 033 */
  { 0 },
/* 034 */
  { 0 },
/* 035 */
  { 0 },
/* 036 */
  { 0 },
/* 037 */
  { 0 },
/* 038 */
  { 0 },
/* 039 */
  { 0 },
/* 040 */
  { 0 },
/* 041 */
  { 0 },
/* 042 */
  { 0 },
/* 043 */
  { 0 },
/* 044 */
  { 0 },
/* 045 */
  { 0 },
/* 046 */
  { 0 },
/* 047 */
  { 0 },
/* 048 */
  { 0 },
/* 049 */
  { 0 },
/* 050 */
  { 0 },
/* 051 */
  { 0 },
/* 052 */
  { 0 },
/* 053 */
  { 0 },
/* 054 */
  { 0 },
/* 055 */
  { 0 },
/* 056 */
  { 0 },
/* 057 */
  { 0 },
/* 058 */
  { 0 },
/* 059 */
  { 0 },
/* 060 */
  { 0 },
/* 061 */
  { 0 },
/* 062 */
  { 0 },
/* 063 */
  { 0 },
/* 064 */
  { 0 },
/* 065 */
  { 0 },
/* 066 */
  { 0 },
/* 067 */
  { 0 },
/* 068 */
  { 0 },
/* 069 */
  { 0 },
/* 070 */
  { 0 },
/* 071 */
  { 0 },
/* 072 */
  { 0 },
/* 073 */
  { 0 },
/* 074 */
  { 0 },
/* 075 */
  { 0 },
/* 076 */
  { 0 },
/* 077 */
  { 0 },
/* 078 */
  { 0 },
/* 079 */
  { 0 },
/* 080 */
  { 0 },
/* 081 */
  { 0 },
/* 082 */
  { 0 },
/* 083 */
  { 0 },
/* 084 */
  { 0 },
/* 085 */
  { 0 },
/* 086 */
  { 0 },
/* 087 */
  { 0 },
/* 088 */
  { 0 },
/* 089 */
  { 0 },
/* 090 */
  { 0 },
/* 091 */
  { 0 },
/* 092 */
  { 0 },
/* 093 */
  { 0 },
/* 094 */
  { 0 },
/* 095 */
  { 0 },
/* 096 */
  { 0 },
/* 097 */
  { 0 },
/* 098 */
  { 0 },
/* 099 */
  { 0 },
/* 100 */
  { 0 },
/* 101 */
  { 0 },
/* 102 */
  { 0 },
/* 103 */
  { 0 },
/* 104 */
  { 0 },
/* 105 */
  { 0 },
/* 106 */
  { 0 },
/* 107 */
  { 0 },
/* 108 */
  { 0 },
/* 109 */
  { 0 },
/* 110 */
  { 0 },
/* 111 */
  { 0 },
/* 112 */
  { 0 },
/* 113 */
  { 0 },
/* 114 */
  { 0 },
/* 115 */
  { 0 },
/* 116 */
  { 0 },
/* 117 */
  { 0 },
/* 118 */
  { 0 },
/* 119 */
  { 0 },
/* 120 */
  { 0 },
/* 121 */
  { 0 },
/* 122 */
  { 0 },
/* 123 */
  { 0 },
/* 124 */
  { 0 },
/* 125 */
  { 0 },
/* 126 */
  { 0 },
/* 127 */
  { 0 },
/* 128 */
  { 0 },
/* 129 */
  { 0 },
/* 130 */
  { 0 },
/* 131 */
  { 0 },
/* 132 */
  { 0 },
/* 133 */
  { 0 },
/* 134 */
  { 0 },
/* 135 */
  { 0 },
/* 136 */
  { 0 },
/* 137 */
  { 0 },
/* 138 */
  { 0 },
/* 139 */
  { 0 },
/* 140 */
  { 0 },
/* 141 */
  { 0 },
/* 142 */
  { 0 },
/* 143 */
  { 0 },
/* 144 */
  { 0 },
/* 145 */
  { 0 },
/* 146 */
  { 0 },
/* 147 */
  { 0 },
/* 148 */
  { 0 },
/* 149 */
  { 0 },
/* 150 */
  { 0 },
/* 151 */
  { 0 },
/* 152 */
  { 0 },
/* 153 */
  { 0 },
/* 154 */
  { 0 },
/* 155 */
  { 0 },
/* 156 */
  { 0 },
/* 157 */
  { 0 },
/* 158 */
  { 0 },
/* 159 */
  { 0 },
/* 160 */
  { 0 },
/* 161 */
  { 0 },
/* 162 */
  { 0 },
/* 163 */
  { 0 },
/* 164 */
  { 0 },
/* 165 */
  { 0 },
/* 166 */
  { 0 },
/* 167 */
  { 0 },
/* 168 */
  { 0 },
/* 169 */
  { 0 },
/* 170 */
  { 0 },
/* 171 */
  { 0 },
/* 172 */
  { 0 },
/* 173 */
  { 0 },
/* 174 */
  { 0 },
/* 175 */
  { 0 },
/* 176 */
  { 0 },
/* 177 */
  { 0 },
/* 178 */
  { 0 },
/* 179 */
  { 0 },
/* 180 */
  { 0 },
/* 181 */
  { 0 },
/* 182 */
  { 0 },
/* 183 */
  { 0 },
/* 184 */
  { 0 },
/* 185 */
  { 0 },
/* 186 */
  { 0 },
/* 187 */
  { 0 },
/* 188 */
  { 0 },
/* 189 */
  { 0 },
/* 190 */
  { 0 },
/* 191 */
  { 0 },
/* 192 */
  { 0 },
/* 193 */
  { 0 },
/* 194 */
  { 0 },
/* 195 */
  { 0 },
/* 196 */
  { 0 },
/* 197 */
  { 0 },
/* 198 */
  { 0 },
/* 199 */
  { 0 },
/* 200 */
  { RPL_TRACELINK, "Link %s.%s %s %s", "200" },
/* 201 */
  { RPL_TRACECONNECTING, "Try. %s %s", "201" },
/* 202 */
  { RPL_TRACEHANDSHAKE, "H.S. %s %s", "202" },
/* 203 */
  { RPL_TRACEUNKNOWN, "???? %s %s", "203" },
/* 204 */
  { RPL_TRACEOPERATOR, "Oper %s %s %ld", "204" },
/* 205 */
  { RPL_TRACEUSER, "User %s %s %ld", "205" },
/* 206 */
  { RPL_TRACESERVER, "Serv %s %dS %dC %s %s!%s@%s %ld %ld", "206" },
/* 207 */
  { 0 },
/* 208 */
  { RPL_TRACENEWTYPE, "<newtype> 0 %s", "208" },
/* 209 */
  { RPL_TRACECLASS, "Class %s %u", "209" },
/* 210 */
  { 0 },
/* 211 */
  { RPL_STATSLINKINFO, 0, "211" },
/* 212 */
  { RPL_STATSCOMMANDS, "%s %u %u", "212" },
/* 213 */
  { RPL_STATSCLINE, "C %s * %d %d %s %s", "213" },
/* 214 */
  { 0 },
/* 215 */
  { RPL_STATSILINE, "I %s%s%s %d %s%s %d %s", "215" },
/* 216 */
  { RPL_STATSKLINE, "%c %s@%s \"%s\" \"%s\" \"%s\" %s %s 0 0", "216" },
/* 217 */
  { RPL_STATSPLINE, "P %d %d %s %s %s", "217" },
/* 218 */
  { RPL_STATSYLINE, "%c %s %d %d %u %u %u %u", "218" },
/* 219 */
  { RPL_ENDOFSTATS, "%s :End of /STATS report", "219" },
/* 220 */
  { RPL_STATSWLINE, "W %s@%s * %s %s%s%s :%s", "220" },
/* 221 */
  { RPL_UMODEIS, "%s", "221" },
/* 222 */
  { RPL_STATSJLINE, "J %s", "222" },
/* 223 */
  { 0 },
/* 224 */
  { 0 },
/* 225 */
  { 0 },
/* 226 */
  { RPL_STATSALINE, "%s", "226" },
/* 227 */
  { 0 },
/* 228 */
  { RPL_STATSQLINE, "Q %s :%s", "228" },
/* 229 */
  { 0 },
/* 230 */
  { 0 },
/* 231 */
  { 0 },
/* 232 */
  { RPL_RULES, ":- %s", "232" },
/* 233 */
  { 0 },
/* 234 */
  { 0 },
/* 235 */
  { 0 },
/* 236 */
  { RPL_STATSVERBOSE, "V :Sent as explicit", "236" },
/* 237 */
  { RPL_STATSENGINE, "%s :Event loop engine", "237" },
/* 238 */
  { RPL_STATSFLINE, "F %s %s", "238" },
/* 239 */
  { 0 },
/* 240 */
  { 0 },
/* 241 */
  { RPL_STATSLLINE, "Module Description EntryPoint", "241" },
/* 242 */
  { RPL_STATSUPTIME, ":Server Up %d days, %d:%02d:%02d", "242" },
/* 243 */
  { RPL_STATSOLINE, "%c %s@%s * %s %s", "243" },
/* 244 */
  { 0 },
/* 245 */
  { RPL_STATSSLINE, "%d %s %s %s@%s", "245" },
/* 246 */
  { RPL_STATSTLINE, "%c %s %s", "246" },
/* 247 */
  { RPL_STATSGLINE, "%c %s%s%s %Tu %Tu %Tu %s%c :%s", "247" },
/* 248 */
  { RPL_STATSULINE, "U %s", "248" },
/* 249 */
  { RPL_STATSDEBUG, 0, "249" },
/* 250 */
  { RPL_STATSCONN, ":Highest connection count: %u (%u clients)", "250" },
/* 251 */
  { RPL_LUSERCLIENT, ":There are %u users and %u invisible on %u servers", "251" },
/* 252 */
  { RPL_LUSEROP, "%u :operator(s) online", "252" },
/* 253 */
  { RPL_LUSERUNKNOWN, "%u :unknown connection(s)", "253" },
/* 254 */
  { RPL_LUSERCHANNELS, "%u :channels formed", "254" },
/* 255 */
  { RPL_LUSERME, ":I have %u clients and %u servers", "255" },
/* 256 */
  { RPL_ADMINME, ":Administrative info about %s", "256" },
/* 257 */
  { RPL_ADMINLOC1, ":%s", "257" },
/* 258 */
  { RPL_ADMINLOC2, ":%s", "258" },
/* 259 */
  { RPL_ADMINEMAIL, ":%s", "259" },
/* 260 */
  { 0 },
/* 261 */
  { 0 },
/* 262 */
  { RPL_TRACEEND, ":End of TRACE", "262" },
/* 263 */
  { 0 },
/* 264 */
  { 0 },
/* 265 */
  { 0 },
/* 266 */
  { 0 },
/* 267 */
  { 0 },
/* 268 */
  { 0 },
/* 269 */
  { 0 },
/* 270 */
  { RPL_PRIVS, "%s :", "270" },
/* 271 */
  { RPL_SILELIST, "%s %s%s", "271" },
/* 272 */
  { RPL_ENDOFSILELIST, "%s :End of Silence List", "272" },
/* 273 */
  { 0 },
/* 274 */
  { 0 },
/* 275 */
  { RPL_STATSDLINE, "%c %s %s", "275" },
/* 276 */
  { RPL_STATSRLINE, "%-9s %-9s %-10s %s", "276" },
/* 277 */
  { 0 },
/* 278 */
  { 0 },
/* 279 */
  { 0 },
/* 280 */
  { RPL_GLIST, "%s%s%s %Tu %Tu %Tu %s %s%c :%s", "280" },
/* 281 */
  { RPL_ENDOFGLIST, ":End of G-line List", "281" },
/* 282 */
  { RPL_JUPELIST, "%s %Tu %s %c :%s", "282" },
/* 283 */
  { RPL_ENDOFJUPELIST, ":End of Jupe List", "283" },
/* 284 */
  { RPL_FEATURE, 0, "284" },
/* 285 */
  { 0 },
/* 286 */
  { RPL_CHKHEAD, ":Information for %s %s", "286" },
/* 287 */
  { RPL_CHANUSER, ": %s %s%s (%s@%s)   (%s) %s", "287" },
/* 288 */
  { 0 },
/* 289 */
  { 0 },
/* 290 */
  { RPL_DATASTR, ":%s", "290" },
/* 291 */
  { RPL_ENDOFCHECK, ":%s", "291" },
/* 292 */
  { ERR_SEARCHNOMATCH, ":%s %s No matching record(s) found", "292" },
/* 293 */
  { 0 },
/* 294 */
  { 0 },
/* 295 */
  { 0 },
/* 296 */
  { 0 },
/* 297 */
  { 0 },
/* 298 */
  { 0 },
/* 299 */
  { 0 },
/* 300 */
  { 0 },
/* 301 */
  { RPL_AWAY, "%s :%s", "301" },
/* 302 */
  { RPL_USERHOST, ":", "302" },
/* 303 */
  { RPL_ISON, ":", "303" },
/* 304 */
  { 0 },
/* 305 */
  { RPL_UNAWAY, ":You are no longer marked as being away", "305" },
/* 306 */
  { RPL_NOWAWAY, ":You have been marked as being away", "306" },
/* 307 */
  { 0 },
/* 308 */
  { RPL_RULESSTART, ":- %s Network Rules - ", "308" },
/* 309 */
  { RPL_ENDOFRULES, ":End of /RULES list.", "309" },
/* 310 */
  { RPL_WHOISSERVICE, "%s :%s", "310" },
/* 311 */
  { RPL_WHOISUSER, "%s %s %s * :%s", "311" },
/* 312 */
  { RPL_WHOISSERVER, "%s %s :%s", "312" },
/* 313 */
  { RPL_WHOISOPERATOR, "%s :%s", "313" },
/* 314 */
  { RPL_WHOWASUSER, "%s %s %s * :%s", "314" },
/* 315 */
  { RPL_ENDOFWHO, "%s :End of /WHO list.", "315" },
/* 316 */
  { RPL_WHOISPRIVDEAF, "%s :does not accept private messages", "316" },
/* 317 */
  { RPL_WHOISIDLE, "%s %ld %ld :seconds idle, signon time", "317" },
/* 318 */
  { RPL_ENDOFWHOIS, "%s :End of /WHOIS list.", "318" },
/* 319 */
  { RPL_WHOISCHANNELS, "%s :%s", "319" },
/* 320 */
  { RPL_WHOISSPECIAL, "%s :%s", "320" },
/* 321 */
  { RPL_LISTSTART, "Channel :Users  Name", "321" },
/* 322 */
  { RPL_LIST, "%s %u :%s", "322" },
/* 323 */
  { RPL_LISTEND, ":End of /LIST", "323" },
/* 324 */
  { RPL_CHANNELMODEIS, "%s %s %s", "324" },
/* 325 */
  { RPL_WHOISWEBIRC, "%s :is connected via %s", "325" },
/* 326 */
  { 0 },
/* 327 */
  { 0 },
/* 328 */
  { 0 },
/* 329 */
  { RPL_CREATIONTIME, "%s %Tu", "329" },
/* 330 */
  { RPL_WHOISACCOUNT, "%s %s :is logged in as", "330" },
/* 331 */
  { RPL_NOTOPIC, "%s :No topic is set.", "331" },
/* 332 */
  { RPL_TOPIC, "%s :%s", "332" },
/* 333 */
  { RPL_TOPICWHOTIME, "%s %s %Tu", "333" },
/* 334 */
  { RPL_LISTUSAGE, ":%s", "334" },
/* 335 */
  { RPL_WHOISACCOUNTONLY, "%s :only accepts messages from registered users", "335" },
/* 336 */
  { RPL_WHOISBOT, "%s :is a bot", "336" },
/* 337 */
  { 0 },
/* 338 */
  { RPL_WHOISACTUALLY, "%s %s@%s %s :Actual user@host, Actual IP", "338" },
/* 339 */
  { 0 },
/* 340 */
  { RPL_USERIP, ":", "340" },
/* 341 */
  { RPL_INVITING, "%s %s", "341" },
/* 342 */
  { 0 },
/* 343 */
  { 0 },
/* 344 */
  { 0 },
/* 345 */
  { RPL_ISSUEDINVITE, "%s %s %s :%s has been invited by %s", "345" },
/* 346 */
  { RPL_INVITELIST, ":%s", "346" },
/* 347 */
  { RPL_ENDOFINVITELIST, ":End of Invite List", "347" },
/* 348 */
  { 0 },
/* 349 */
  { 0 },
/* 350 */
  { 0 },
/* 351 */
  { RPL_VERSION, "%s+[%s].%s %s :%s", "351" },
/* 352 */
  { RPL_WHOREPLY, "%s", "352" },
/* 353 */
  { RPL_NAMREPLY, "%s", "353" },
/* 354 */
  { RPL_WHOSPCRPL, "%s", "354" },
/* 355 */
  { RPL_DELNAMREPLY, "%s", "355" },
/* 356 */
  { 0 },
/* 357 */
  { 0 },
/* 358 */
  { 0 },
/* 359 */
  { 0 },
/* 360 */
  { 0 },
/* 361 */
  { 0 },
/* 362 */
  { RPL_CLOSING, "%s :Operator enforced Close", "362" },
/* 363 */
  { RPL_CLOSEEND, "%d :Connections Closed", "363" },
/* 364 */
  { RPL_LINKS, "%s %s :%u P%u %s", "364" },
/* 365 */
  { RPL_ENDOFLINKS, "%s :End of /LINKS list.", "365" },
/* 366 */
  { RPL_ENDOFNAMES, "%s :End of /NAMES list.", "366" },
/* 367 */
  { RPL_BANLIST, "%s %s %s %Tu", "367" },
/* 368 */
  { RPL_ENDOFBANLIST, "%s :End of Channel Ban List", "368" },
/* 369 */
  { RPL_ENDOFWHOWAS, "%s :End of WHOWAS", "369" },
/* 370 */
  { 0 },
/* 371 */
  { RPL_INFO, ":%s", "371" },
/* 372 */
  { RPL_MOTD, ":- %s", "372" },
/* 373 */
  { 0 },
/* 374 */
  { RPL_ENDOFINFO, ":End of /INFO list.", "374" },
/* 375 */
  { RPL_MOTDSTART, ":- %s Message of the Day - ", "375" },
/* 376 */
  { RPL_ENDOFMOTD, ":End of /MOTD command.", "376" },
/* 377 */
  { 0 },
/* 378 */
  { 0 },
/* 379 */
  { 0 },
/* 380 */
  { 0 },
/* 381 */
  { RPL_YOUREOPER, ":You are now an IRC Operator", "381" },
/* 382 */
  { RPL_REHASHING, "%s :Rehashing", "382" },
/* 383 */
  { 0 },
/* 384 */
  { 0 },
/* 385 */
  { 0 },
/* 386 */
  { 0 },
/* 387 */
  { 0 },
/* 388 */
  { 0 },
/* 389 */
  { 0 },
/* 390 */
  { 0 },
/* 391 */
  { RPL_TIME, "%s %Tu %ld :%s", "391" },
/* 392 */
  { 0 },
/* 393 */
  { 0 },
/* 394 */
  { 0 },
/* 395 */
  { 0 },
/* 396 */
  { RPL_HOSTHIDDEN, "%s :is now your%s host", "396" },
/* 397 */
  { 0 },
/* 398 */
  { 0 },
/* 399 */
  { 0 },
/* 400 */
  { 0 },
/* 401 */
  { ERR_NOSUCHNICK, "%s :No such nick", "401" },
/* 402 */
  { ERR_NOSUCHSERVER, "%s :No such server", "402" },
/* 403 */
  { ERR_NOSUCHCHANNEL, "%s :No such channel", "403" },
/* 404 */
  { ERR_CANNOTSENDTOCHAN, "%s :Cannot send to channel", "404" },
/* 405 */
  { ERR_TOOMANYCHANNELS, "%s :You have joined too many channels", "405" },
/* 406 */
  { ERR_WASNOSUCHNICK, "%s :There was no such nickname", "406" },
/* 407 */
  { ERR_TOOMANYTARGETS, "%s :Duplicate recipients. No message delivered", "407" },
/* 408 */
  { 0 },
/* 409 */
  { ERR_NOORIGIN, ":No origin specified", "409" },
/* 410 */
  { ERR_UNKNOWNCAPCMD, "%s :Unknown CAP subcommand", "410" },
/* 411 */
  { ERR_NORECIPIENT, ":No recipient given (%s)", "411" },
/* 412 */
  { ERR_NOTEXTTOSEND, ":No text to send", "412" },
/* 413 */
  { ERR_NOTOPLEVEL, "%s :No toplevel domain specified", "413" },
/* 414 */
  { ERR_WILDTOPLEVEL, "%s :Wildcard in toplevel Domain", "414" },
/* 415 */
  { 0 },
/* 416 */
  { ERR_QUERYTOOLONG, "%s :Too many lines in the output, restrict your query", "416" },
/* 417 */
  { ERR_INPUTTOOLONG, ":Input line was too long", "417" },
/* 418 */
  { 0 },
/* 419 */
  { 0 },
/* 420 */
  { 0 },
/* 421 */
  { ERR_UNKNOWNCOMMAND, "%s :Unknown command", "421" },
/* 422 */
  { ERR_NOMOTD, ":MOTD File is missing", "422" },
/* 423 */
  { ERR_NOADMININFO, "%s :No administrative info available", "423" },
/* 424 */
  { 0 },
/* 425 */
  { 0 },
/* 426 */
  { 0 },
/* 427 */
  { 0 },
/* 428 */
  { 0 },
/* 429 */
  { 0 },
/* 430 */
  { 0 },
/* 431 */
  { ERR_NONICKNAMEGIVEN, ":No nickname given", "431" },
/* 432 */
  { ERR_ERRONEUSNICKNAME, "%s :Erroneous Nickname", "432" },
/* 433 */
  { ERR_NICKNAMEINUSE, "%s :Nickname is already in use.", "433" },
/* 434 */
  { ERR_NORULES, ":RULES File is missing", "434" },
/* 435 */
  { 0 },
/* 436 */
  { ERR_NICKCOLLISION, "%s :Nickname collision KILL", "436" },
/* 437 */
  { ERR_BANNICKCHANGE, "%s :Cannot change nickname while banned on channel or channel is moderated", "437" },
/* 438 */
  { ERR_NICKTOOFAST, "%s :Nick change too fast. Please wait %d seconds.", "438" },
/* 439 */
  { ERR_TARGETTOOFAST, "%s :Target change too fast. Please wait %d seconds.", "439" },
/* 440 */
  { ERR_SERVICESDOWN, "%s :Services are currently unavailable.", "440" },
/* 441 */
  { ERR_USERNOTINCHANNEL, "%s %s :They aren't on that channel", "441" },
/* 442 */
  { ERR_NOTONCHANNEL, "%s :You're not on that channel", "442" },
/* 443 */
  { ERR_USERONCHANNEL, "%s %s :is already on channel", "443" },
/* 444 */
  { 0 },
/* 445 */
  { 0 },
/* 446 */
  { 0 },
/* 447 */
  { 0 },
/* 448 */
  { 0 },
/* 449 */
  { 0 },
/* 450 */
  { 0 },
/* 451 */
  { ERR_NOTREGISTERED, ":You have not registered", "451" },
/* 452 */
  { 0 },
/* 453 */
  { 0 },
/* 454 */
  { 0 },
/* 455 */
  { 0 },
/* 456 */
  { 0 },
/* 457 */
  { 0 },
/* 458 */
  { 0 },
/* 459 */
  { 0 },
/* 460 */
  { 0 },
/* 461 */
  { ERR_NEEDMOREPARAMS, "%s :Not enough parameters", "461" },
/* 462 */
  { ERR_ALREADYREGISTRED, ":You may not reregister", "462" },
/* 463 */
  { ERR_NOPERMFORHOST, ":Your host isn't among the privileged", "463" },
/* 464 */
  { ERR_PASSWDMISMATCH, ":Password Incorrect", "464" },
/* 465 */
  { ERR_YOUREBANNEDCREEP, ":You are banned from this server", "465" },
/* 466 */
  { ERR_YOUWILLBEBANNED, "", "466" },
/* 467 */
  { ERR_KEYSET, "%s :Channel key already set", "467" },
/* 468 */
  { ERR_INVALIDUSERNAME, 0, "468" },
/* 469 */
  { ERR_SSLONLYCHAN, "%s :Cannot join channel (+Z)", "469" },
/* 470 */
  { ERR_OPERONLYCHAN, "%s :Cannot join channel (+O)", "470" },
/* 471 */
  { ERR_CHANNELISFULL, "%s :Cannot join channel (+l)", "471" },
/* 472 */
  { ERR_UNKNOWNMODE, "%c :is unknown mode char to me", "472" },
/* 473 */
  { ERR_INVITEONLYCHAN, "%s :Cannot join channel (+i)", "473" },
/* 474 */
  { ERR_BANNEDFROMCHAN, "%s :Cannot join channel (+b)", "474" },
/* 475 */
  { ERR_BADCHANNELKEY, "%s :Cannot join channel (+k)", "475" },
/* 476 */
  { ERR_BADCHANMASK, "%s :Bad Channel Mask", "476" },
/* 477 */
  { ERR_NEEDREGGEDNICK, "%s :Cannot join channel (+r): this channel requires authentication -- you can obtain an account from %s", "477" },
/* 478 */
  { ERR_BANLISTFULL, "%s %s :Channel ban/ignore list is full", "478" },
/* 479 */
  { ERR_BADCHANNAME, "%s :Cannot join channel (access denied on this server)", "479" },
/* 480 */
  { ERR_ADMINONLYCHAN, "%s :Cannot join channel (+a)", "480" },
/* 481 */
  { ERR_NOPRIVILEGES, ":Permission Denied: Insufficient privileges", "481" },
/* 482 */
  { ERR_CHANOPRIVSNEEDED, "%s :You're not channel operator", "482" },
/* 483 */
  { ERR_CANTKILLSERVER, ":You cant kill a server!", "483" },
/* 484 */
  { ERR_ISCHANSERVICE, "%s %s :Cannot kill, kick or deop %s", "484" },
/* 485 */
  { ERR_COMMONCHANSONLY, "%s :You must share at least one channel with this user in order to %s them", "485" },
/* 486 */
  { ERR_ACCOUNTONLY, "%s :You need to login to services to %s %s", "486" },
/* 487 */
  { ERR_PRIVDEAF, "%s :%s to '%s' not delivered: User does not accept private messages", "487" },
/* 488 */
  { 0 },
/* 489 */
  { ERR_VOICENEEDED, "%s :You're neither voiced nor channel operator", "489" },
/* 490 */
  { 0 },
/* 491 */
  { ERR_NOOPERHOST, ":No Operator block for your host", "491" },
/* 492 */
  { 0 },
/* 493 */
  { ERR_NOFEATURE, "%s :No such feature", "493" },
/* 494 */
  { ERR_BADFEATVALUE, "%s :Bad value for feature %s", "494" },
/* 495 */
  { ERR_BADLOGTYPE, "%s :No such log type", "495" },
/* 496 */
  { ERR_BADLOGSYS, "%s :No such log subsystem", "496" },
/* 497 */
  { ERR_BADLOGVALUE, "%s :Bad value for log type", "497" },
/* 498 */
  { ERR_ISOPERLCHAN, "%s %s :Cannot kick or deop an IRC Operator on a local channel", "498" },
/* 499 */
  { 0 },
/* 500 */
  { 0 },
/* 501 */
  { ERR_UMODEUNKNOWNFLAG, "%c :Unknown user MODE flag", "501" },
/* 502 */
  { ERR_USERSDONTMATCH, ":Cant change mode for other users", "502" },
/* 503 */
  { 0 },
/* 504 */
  { 0 },
/* 505 */
  { 0 },
/* 506 */
  { 0 },
/* 507 */
  { 0 },
/* 508 */
  { 0 },
/* 509 */
  { 0 },
/* 510 */
  { 0 },
/* 511 */
  { ERR_SILELISTFULL, "%s :Your silence list is full", "511" },
/* 512 */
  { ERR_NOSUCHGLINE, "%s :No such gline", "512" },
/* 513 */
  { ERR_BADPING, 0, "513" },
/* 514 */
  { ERR_NOSUCHJUPE, "%s :No such jupe", "514" },
/* 515 */
  { ERR_BADEXPIRE, "%Tu :Bad expire time", "515" },
/* 516 */
  { ERR_DONTCHEAT, "%s :Don't Cheat.", "516" },
/* 517 */
  { ERR_DISABLED, "%s :Command disabled.", "517" },
/* 518 */
  { ERR_LONGMASK, ":Mask is too long", "518" },
/* 519 */
  { ERR_TOOMANYUSERS, "%d :Too many users affected by mask", "519" },
/* 520 */
  { ERR_MASKTOOWIDE, "%s :Mask is too wide", "520" },
/* 521 */
  { 0 },
/* 522 */
  { 0 },
/* 523 */
  { 0 },
/* 524 */
  { ERR_QUARANTINED, "%s :Channel is quarantined : %s", "524" },
/* 525 */
  { ERR_INVALIDKEY, "%s :Key is not well-formed", "525" },
/* 526 */
  { 0 },
/* 527 */
  { 0 },
/* 528 */
  { 0 },
/* 529 */
  { 0 },
/* 530 */
  { ERR_BADHOSTMASK, "%s :Invalid username/hostmask", "530" },
/* 531 */
  { ERR_HOSTUNAVAIL, "%s :Sethost not found or unavailable", "531" },
/* 532 */
  { 0 },
/* 533 */
  { 0 },
/* 534 */
  { 0 },
/* 535 */
  { RPL_OMOTDSTART, ":%s IRC Operator MOTD", "535" },
/* 536 */
  { RPL_OMOTD, ":- %s", "536" },
/* 537 */
  { RPL_ENDOFOMOTD, ":End of IRC Operator MOTD.", "537" },
/* 538 */
  { 0 },
/* 539 */
  { 0 },
/* 540 */
  { 0 },
/* 541 */
  { 0 },
/* 542 */
  { RPL_STATSSHUN, "%c %s%s%s %Tu %Tu %c :%s", "542" },
/* 543 */
  { ERR_NOSUCHSHUN, "%s :No such shun", "543" },
/* 544 */
  { 0 },
/* 545 */
  { RPL_ENDOFSLIST, ":End of Shun List", "545" },
/* 546 */
  { 0 },
/* 547 */
  { 0 },
/* 548 */
  { 0 },
/* 549 */
  { 0 },
/* 550 */
  { 0 },
/* 551 */
  { 0 },
/* 552 */
  { 0 },
/* 553 */
  { 0 },
/* 554 */
  { 0 },
/* 555 */
  { 0 },
/* 556 */
  { 0 },
/* 557 */
  { 0 },
/* 558 */
  { 0 },
/* 559 */
  { 0 },
/* 560 */
  { ERR_NOTLOWEROPLEVEL, "%s %s %hu %hu :Cannot %s someone with %s op-level", "560" },
/* 561 */
  { ERR_NOTMANAGER, "%s :You must be channel Admin to add or remove a password. Use /JOIN %s <AdminPass>.", "561" },
/* 562 */
  { ERR_CHANSECURED, "%s :Channel is older than 48 hours and secured. Cannot change Admin pass anymore", "562" },
/* 563 */
  { ERR_UPASSSET, "%s :Cannot remove Admin pass (+A) while User pass (+U) is still set. First use /MODE %s -U <userpass>", "563" },
/* 564 */
  { ERR_UPASSNOTSET, "%s :Cannot set user pass (+U) until Admin pass (+A) is set. First use /MODE %s +A <adminpass>", "564" },
/* 565 */
  { 0 },
/* 566 */
  { ERR_NOMANAGER, "%s :Re-create the channel. The channel must be completely empty for a period of %s before it can be recreated.", "566" },
/* 567 */
  { ERR_UPASS_SAME_APASS, "%s :Cannot use the same pass for both admin (+A) and user (+U) pass.", "567" },
/* 568 */
  { ERR_NOOMOTD, ":IRC Operator MOTD File is missing", "568" },
/* 569 */
  { 0 },
/* 570 */
  { 0 },
/* 571 */
  { 0 },
/* 572 */
  { 0 },
/* 573 */
  { 0 },
/* 574 */
  { 0 },
/* 575 */
  { 0 },
/* 576 */
  { 0 },
/* 577 */
  { 0 },
/* 578 */
  { 0 },
/* 579 */
  { 0 },
/* 580 */
  { 0 },
/* 581 */
  { 0 },
/* 582 */
  { 0 },
/* 583 */
  { 0 },
/* 584 */
  { 0 },
/* 585 */
  { 0 },
/* 586 */
  { 0 },
/* 587 */
  { 0 },
/* 588 */
  { 0 },
/* 589 */
  { 0 },
/* 590 */
  { 0 },
/* 591 */
  { 0 },
/* 592 */
  { 0 },
/* 593 */
  { 0 },
/* 594 */
  { 0 },
/* 595 */
  { 0 },
/* 596 */
  { 0 },
/* 597 */
  { 0 },
/* 598 */
  { 0 },
/* 599 */
  { 0 },
/* 600 */
  { 0 },
/* 601 */
  { 0 },
/* 602 */
  { 0 },
/* 603 */
  { 0 },
/* 604 */
  { 0 },
/* 605 */
  { 0 },
/* 606 */
  { 0 },
/* 607 */
  { 0 },
/* 608 */
  { 0 },
/* 609 */
  { 0 },
/* 610 */
  { 0 },
/* 611 */
  { 0 },
/* 612 */
  { 0 },
/* 613 */
  { RPL_SSLFP, "%s :SSL fingerprint: %s", "613" },
/* 614 */
  { ERR_NOSSLFP, "%s :No SSL fingerprint", "614" },
/* 615 */
  { 0 },
/* 616 */
  { RPL_WHOISSSLFP, "%s :has client certificate fingerprint %s", "616" },
/* 617 */
  { 0 },
/* 618 */
  { 0 },
/* 619 */
  { 0 },
/* 620 */
  { 0 },
/* 621 */
  { 0 },
/* 622 */
  { 0 },
/* 623 */
  { 0 },
/* 624 */
  { 0 },
/* 625 */
  { 0 },
/* 626 */
  { 0 },
/* 627 */
  { 0 },
/* 628 */
  { 0 },
/* 629 */
  { 0 },
/* 630 */
  { 0 },
/* 631 */
  { 0 },
/* 632 */
  { 0 },
/* 633 */
  { 0 },
/* 634 */
  { 0 },
/* 635 */
  { 0 },
/* 636 */
  { 0 },
/* 637 */
  { 0 },
/* 638 */
  { 0 },
/* 639 */
  { 0 },
/* 640 */
  { 0 },
/* 641 */
  { 0 },
/* 642 */
  { 0 },
/* 643 */
  { 0 },
/* 644 */
  { 0 },
/* 645 */
  { 0 },
/* 646 */
  { 0 },
/* 647 */
  { 0 },
/* 648 */
  { 0 },
/* 649 */
  { 0 },
/* 650 */
  { 0 },
/* 651 */
  { 0 },
/* 652 */
  { 0 },
/* 653 */
  { 0 },
/* 654 */
  { 0 },
/* 655 */
  { 0 },
/* 656 */
  { 0 },
/* 657 */
  { 0 },
/* 658 */
  { 0 },
/* 659 */
  { 0 },
/* 660 */
  { 0 },
/* 661 */
  { 0 },
/* 662 */
  { 0 },
/* 663 */
  { 0 },
/* 664 */
  { 0 },
/* 665 */
  { 0 },
/* 666 */
  { 0 },
/* 667 */
  { 0 },
/* 668 */
  { 0 },
/* 669 */
  { 0 },
/* 670 */
  { 0 },
/* 671 */
  { RPL_WHOISSSL, "%s :is connected via SSL", "671" }
};

/** Return a pointer to the Numeric for a particular code.
 * @param n %Numeric to look up.
 * @return Numeric structure.
 */
const struct Numeric* get_error_numeric(int n)
{
  assert(0 < n);
  assert(n < ERR_LASTERROR);
  assert(0 != replyTable[n].value);

  return &replyTable[n];
}

/** Return a format string for a numeric response.
 * @param n %Numeric to look up.
 * @return Pointer to a static buffer containing the format string.
 */
char* rpl_str(int n)
{
  static char numbuff[512];
  Numeric* p;

  assert(0 < n);
  assert(n < ERR_LASTERROR);
  assert(0 != replyTable[n].value);

  p = &replyTable[n];
  strcpy(numbuff, ":%s 000 %s ");
  if (p->str) {
    numbuff[4] = p->str[0];
    numbuff[5] = p->str[1];
    numbuff[6] = p->str[2];
    strcpy(numbuff + 11, p->format);
  }

  return numbuff;
}

