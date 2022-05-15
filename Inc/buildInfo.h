/*
MIT License

Copyright (c) 2022 Agvolution GmbH
Head Development: Lukas Kamm / l.kamm@agvolution.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _BUILDINFO_H
#define _BUILDINFO_H

#define BUILDINFO_IDENTIFIER 0xcafecafe

typedef struct __attribute__((packed)) __buildInfo_t  {
	uint32_t	identifier;		// Identifier. Used to locate buildInfo in binary
	const char	*naming;		// Name of this build
	const char	*vendor;		// Vendor of this build
	uint8_t		v_major;		// Major version
	uint8_t		v_minor;		// Minor version
	uint8_t		v_patch;		// Patch version
	const char	*buildDate;		// Build date in __DATE__ Format (mmm dd yyyy)
	const char	*buildTime;		// Build time in __TIME__ Format (hh:mm:ss)
} buildInfo_t;

#endif // _BUILDINFO_H