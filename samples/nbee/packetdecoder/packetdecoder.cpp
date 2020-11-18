/*****************************************************************************/
/*                                                                           */
/* Copyright notice: please read file license.txt in the NetBee root folder. */
/*                                                                           */
/*****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nbee.h>
#include <vector>
#include <string>

#define PDMLFILE "pdmlout.xml"
#define PSMLFILE "psmlout.xml"

struct pcap_pkthdr { // FIXME HACK
	struct timeval ts;	/* time stamp */
	uint32_t caplen;	/* length of portion present */
	uint32_t len;	/* length this packet (off wire) */
};

// Global variables for configuration
int ShowNetworkNames;
char* NetPDLFileName;
char* CaptureFileName;
int DecodingType;
int StoreRawDump;


enum {
	DECODEALL_AND_SAVE = 0,
	DECODE_AND_SAVE,
	DECODE_NOSAVE,
	DECODEALL_NOSAVE
};


void Usage()
{
char string[]= \
	"\nUsage:\n"	\
	"  packetdecoder [options]\n\n"	\
	"Options:\n"	\
	" -shownetworknames: shows network names (e.g. www.foo.com) instead of IP\n"		\
	"     addresses in the decoded file. Default: disabled.\n"							\
	" -netpdl FileName: name (and *path*) of the file containing the NetPDL\n"			\
	"     description. In case it is omitted, the NetPDL description embedded\n"		\
	"     within the NetBee library will be used.\n"									\
	" -capturefile FileName: name (and *path*) of the file containing the packet\n"		\
	"     dump that has to be decoded. If missing, file 'samplecapturedump.acp'\n"		\
	"     will be used.\n"																\
	" -mindecode: this program dumps the PDML result on file, but it uses\n"			\
	"     a minimal database, which excludes visualization primitives.\n"				\
	"     Please note that the PSML will not be generated.\n"							\
	"     Useful mostly when using a reduced version of the database.\n"				\
	" -mindecode_nosave: this program does not save any PDML/PSML result on file.\n"	\
	"     In addition, it uses a minimal database, which excludes visualization\n"		\
	"     primitives. Useful mostly for debug/performance purposes.\n"					\
	" -fulldecode_nosave: this program does not save any PDML/PSML result \n"			\
	"     on file. However, the complete decoding (i.e. including visualization\n"		\
	"     primitives) is done. Useful mostly for debug/performance purposes.\n"			\
	" -rawdump: in addition to the previous options, this switch will enable the\n"		\
	"     generation of the raw dump in the PDML fragments \n"							\
	" -h: prints this help message.\n\n"												\
	"Description\n"																		\
	"============================================================================\n"	\
	"This program shows how to use the NetBee Packet Decoder and to dump the result\n"	\
	"  on disk.\n"																		\
	"This program opens a capture dump file, it decodes all the packets, and it\n"		\
	"  creates the corresponding PSML and PDML files (named '" PDMLFILE "' and \n"		\
	"  '" PSMLFILE "' that are saved on disk in the current folder.\n"					\
	"By default, it creates the PDML/PSML fragments using NetPDL visualization\n"		\
	"  primitives.\n\n";

	printf("%s", string);
}


int ParseCommandLine(int argc, char *argv[])
{
int CurrentItem;
	
	CurrentItem= 1;

	while (CurrentItem < argc)
	{
		if (strcmp(argv[CurrentItem], "-shownetworknames") == 0)
		{
			ShowNetworkNames= 1;
			CurrentItem++;
			continue;
		}

		if (strcmp(argv[CurrentItem], "-netpdl") == 0)
		{
			NetPDLFileName= argv[CurrentItem+1];
			CurrentItem+= 2;
			continue;
		}

		if (strcmp(argv[CurrentItem], "-capturefile") == 0)
		{
			CaptureFileName= argv[CurrentItem+1];
			CurrentItem+= 2;
			continue;
		}

		if (strcmp(argv[CurrentItem], "-mindecode") == 0)
		{
			if (DecodingType)
			{
				printf("Error: option '%s' cannot be used with '-mindecode_nosave' or '-fulldecode_nosave'.\n", argv[CurrentItem]);
				return nbFAILURE;
			}
			DecodingType= DECODE_AND_SAVE;
			CurrentItem++;
			continue;
		}

		if (strcmp(argv[CurrentItem], "-mindecode_nosave") == 0)
		{
			if (DecodingType)
			{
				printf("Error: option '%s' cannot be used with '-mindecode' or '-fulldecode_nosave'.\n", argv[CurrentItem]);
				return nbFAILURE;
			}
			DecodingType= DECODE_NOSAVE;
			CurrentItem++;
			continue;
		}

		if (strcmp(argv[CurrentItem], "-fulldecode_nosave") == 0)
		{
			if (DecodingType)
			{
				printf("Error: option '%s' cannot be used with '-mindecode' or '-mindecode_nosave'.\n", argv[CurrentItem]);
				return nbFAILURE;
			}
			DecodingType= DECODEALL_NOSAVE;
			CurrentItem++;
			continue;
		}

		if (strcmp(argv[CurrentItem], "-rawdump") == 0)
		{
			StoreRawDump= 1;
			CurrentItem++;
			continue;
		}

		if (strcmp(argv[CurrentItem], "-h") == 0)
		{
			Usage();
			return nbFAILURE;
		}

		printf("Error: parameter '%s' is not valid.\n", argv[CurrentItem]);
		return nbFAILURE;
	}

	return nbSUCCESS;
}


int main(int argc, char *argv[])
{
nbPacketDecoder *Decoder;
nbPacketDecoderVars* PacketDecoderVars;
char ErrBuf[2048 + 1];
char Buffer[2048];
nbNetPDLLinkLayer_t LinkLayerType = nbNETPDL_LINK_LAYER_ETHERNET;
int PacketCounter= 1;
int NetPDLProtoDBFlags;
int NetPDLDecoderFlags;

	if (ParseCommandLine(argc, argv) == nbFAILURE)
		return nbFAILURE;

	printf("\n\nLoading NetPDL protocol database...\n");

	if ((DecodingType == DECODEALL_AND_SAVE) || (DecodingType == DECODEALL_NOSAVE))
		NetPDLProtoDBFlags= nbPROTODB_FULL;
	else
		NetPDLProtoDBFlags= nbPROTODB_MINIMAL;

	if (NetPDLFileName)
	{
	int Res;

		Res= nbInitialize(NetPDLFileName, NetPDLProtoDBFlags, ErrBuf, sizeof(ErrBuf) );

		if (Res == nbFAILURE)
		{
			printf("Error initializing the NetBee Library; %s\n", ErrBuf);
			printf("\n\nUsing the NetPDL database embedded in the NetBee library instead.\n");
		}
	}

	// In case the NetBee library has not been initialized,
	// initialize right now with the embedded NetPDL protocol database instead
	if (nbIsInitialized() == nbFAILURE)
	{
		if (nbInitialize(NULL, NetPDLProtoDBFlags, ErrBuf, sizeof(ErrBuf)) == nbFAILURE)
		{
			printf("Error initializing the NetBee Library; %s\n", ErrBuf);
			return nbFAILURE;
		}
	}

	printf("NetPDL Protocol database loaded.\n");

	if (StoreRawDump)
		NetPDLDecoderFlags= nbDECODER_GENERATEPDML_RAWDUMP;
	else
		NetPDLDecoderFlags= 0;

	// Create a NetPDL Parser to decode packet
	switch(DecodingType)
	{
		case DECODE_NOSAVE:
		{
			NetPDLDecoderFlags|= nbDECODER_GENERATEPDML;
		}; break;

		case DECODE_AND_SAVE:
		{
			NetPDLDecoderFlags|= (nbDECODER_GENERATEPDML | nbDECODER_KEEPALLPDML);
		}; break;

		case DECODEALL_NOSAVE:
		{
			NetPDLDecoderFlags|= (nbDECODER_GENERATEPDML_COMPLETE | nbDECODER_GENERATEPSML);
		}; break;

		default:
		{
			NetPDLDecoderFlags|= (nbDECODER_GENERATEPDML_COMPLETE | nbDECODER_GENERATEPSML |
								nbDECODER_KEEPALLPSML | nbDECODER_KEEPALLPDML);
		}; break;
	}

	Decoder= nbAllocatePacketDecoder(NetPDLDecoderFlags, ErrBuf, sizeof(ErrBuf));
	if (Decoder == NULL)
	{
		printf("Error creating the NetPDLParser: %s.\n", ErrBuf);
		return nbFAILURE;
	}

	// Let's set the source file
	if (CaptureFileName)
		strcpy(Buffer, CaptureFileName);
	else
		strcpy(Buffer, "samplecapturedump.acp");



	// Get the PacketDecoderVars; let's do the check, although it is not really needed
	if ((PacketDecoderVars= Decoder->GetPacketDecoderVars()) == NULL)
	{
		printf("Error: cannot get an instance of the nbPacketDecoderVars class.\n");
		return nbFAILURE;
	}

	// Set the appropriate NetPDL configuration variables
	PacketDecoderVars->SetVariableNumber((char*) NETPDL_VARIABLE_SHOWNETWORKNAMES, ShowNetworkNames);

	printf("\nStarting the file processing...\n\n");

	// Without the optional PCAP dependency, you will need to read your packets on your own. For demonstration purposes,
	// we use a plain array.
	std::vector<std::pair<const char*, size_t>> packets = {
		{"\xff\xff\xff\xff\xff\xff\x00\x80\xc7\xcb\x43\x9a\x08\x06\x00\x01\x08\x00\x06\x04\x00\x01\x00\x80\xc7\xcb\x43\x9a\x82\xc0"
		 "\x10\x51\x00\x00\x00\x00\x00\x00\x82\xc0\x10\x11", 42},
		{"\x00\x80\xc7\xcb\x43\x9a\x00\xe0\x1e\xec\x3c\x84\x08\x06\x00\x01\x08\x00\x06\x04\x00\x02\x00\xe0\x1e\xec\x3c\x84\x82\xc0"
		 "\x10\x11\x00\x80\xc7\xcb\x43\x9a\x82\xc0\x10\x51\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",60},
		{"\x00\xe0\x1e\xec\x3c\x84\x00\x80\xc7\xcb\x43\x9a\x08\x00\x45\x00\x00\x3c\x04\x46\x00\x00\x80\x01\xd8\xbf\x82\xc0\x10\x51"
		 "\xc0\xa8\x0a\x02\x08\x00\x24\x5c\x02\x00\x27\x00\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72"
		 "\x73\x74\x75\x76\x77\x61\x62\x63\x64\x65\x66\x67\x68\x69", 74},
		{"\x00\x06\x29\x99\x2d\xa3\x00\xe0\x1e\xec\x3c\x84\x08\x00\x45\x00\x00\x3c\x04\x46\x00\x00\x7f\x01\xd9\xbf\x82\xc0\x10\x51"
		 "\xc0\xa8\x0a\x02\x08\x00\x24\x5c\x02\x00\x27\x00\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72"
		 "\x73\x74\x75\x76\x77\x61\x62\x63\x64\x65\x66\x67\x68\x69", 74},
		{"\x00\xe0\x1e\xec\x3c\x84\x00\x06\x29\x99\x2d\xa3\x08\x00\x45\x00\x00\x3c\xe5\x02\x00\x00\x80\x01\xf8\x02\xc0\xa8\x0a\x02"
		 "\x82\xc0\x10\x51\x00\x00\x2c\x5c\x02\x00\x27\x00\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x61\x62\x63\x64\x65\x66\x67\x68\x69", 74},
		{"\x00\x80\xc7\xcb\x43\x9a\x00\xe0\x1e\xec\x3c\x84\x08\x00\x45\x00\x00\x3c\xe5\x02\x00\x00\x7f\x01\xf9\x02\xc0\xa8\x0a\x02"
		 "\x82\xc0\x10\x51\x00\x00\x2c\x5c\x02\x00\x27\x00\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72"
		 "\x73\x74\x75\x76\x77\x61\x62\x63\x64\x65\x66\x67\x68\x69", 74}
	};
	for (const auto& packet : packets)
	{
		int RetVal;
		struct pcap_pkthdr PktHeader;
		PktHeader.ts.tv_sec=42;
		PktHeader.ts.tv_usec=0;
		PktHeader.len = PktHeader.caplen = packet.second;

		// Decode packet
		if (Decoder->DecodePacket(LinkLayerType, PacketCounter, &PktHeader, reinterpret_cast<const unsigned char*>(packet.first)) == nbFAILURE)
		{
			printf("\nError decoding a packet %s\n\n", Decoder->GetLastError());
			// Let's break and save what we've done so far
			break;
		}

		PacketCounter++;
	}

	printf("\nRead and decoded %d packets.\n\n", PacketCounter - 1);


	// Dump files to disk
	if ((DecodingType != DECODE_NOSAVE) && (DecodingType != DECODEALL_NOSAVE))
	{
		printf("\nDumping PDML file on disk: file %s\n", PDMLFILE);

		nbPDMLReader *PDMLReader= Decoder->GetPDMLReader();
		if (PDMLReader == NULL)
		{
			printf("Error getting PDMLReader: %s\n", Decoder->GetLastError() );
			return nbFAILURE;
		}

		if (PDMLReader->SaveDocumentAs(PDMLFILE) == nbFAILURE)
		{
			printf("Error dumping PDML file on disk: %s\n", PDMLReader->GetLastError());
			return nbFAILURE;
		}


		if (DecodingType != DECODE_AND_SAVE)
		{
			printf("\nDumping PSML file on disk: file %s\n", PSMLFILE);

			nbPSMLReader *PSMLReader= Decoder->GetPSMLReader();
			if (PSMLReader == NULL)
			{
				printf("Error getting PSMLReader: %s\n", Decoder->GetLastError() );
				return nbFAILURE;
			}

			if (PSMLReader->SaveDocumentAs(PSMLFILE) == nbFAILURE)
			{
				printf("Error dumping PSML file on disk: %s\n", PSMLReader->GetLastError());
				return nbFAILURE;
			}
		}
	}

	// delete the decoder
	nbDeallocatePacketDecoder(Decoder);

	nbCleanup();

	return nbSUCCESS;
}
