##############################################################################
### startup code for PSP
##############################################################################

		.set noreorder

		.text

		.extern xinit
		.extern xmain

		.globl	_start
		.globl	__moduleinfo

##############################################################################
### start

		.ent _start
		.weak _start
_start: 

      #addiu  $sp, 0x10 
      #sw     $31, 0($sp) 
      #sw     $16, 4($sp) 
      #sw     $17, 8($sp) 

      addiu  $sp, -0x10 
      sw     $31, 0($sp)
      sw     $4, 4($sp) 
      sw     $5, 8($sp) 

      la     $2,_gp
      move   $gp,$2

      move   $16, $4            # Save args 
      move   $17, $5 

      la     $4, _main_thread_name   # Main thread setup 
      la     $5, xmain 
      li     $6, 0x20               # Priority 
#      li     $7, 0x40000            # Stack size 
      li     $7, 0xFA0              # Stack size 

#ifdef _NON_KERNEL
      lui    $8, 0x8000             # Attributes (user mode)
#else
	  or     $5, 0x80000000         # for kernel mode
      lui    $8, 0x0000             # Attributes (for kernel mode)
#endif

      jal    sceKernelCreateThread 
      move   $9, $0 

      move   $4, $2            # Start thread 
      move   $5, $16
      jal    sceKernelStartThread 
      move   $6, $17

      lw     $ra, 0($sp) 
      lw     $16, 4($sp) 
      lw     $17, 8($sp) 
      move   $2 , $0 
      jr     $ra 
      addiu  $sp, 0x10 

_main_thread_name: 
      .asciiz   "user_main" 


##############################################################################
### lib entry

		.section	.lib.ent,"wa",@progbits
__lib_ent_top:
		.word 0
		.word 0x80000000
		.word 0x00010104
		.word __entrytable


		.section	.lib.ent.btm,"wa",@progbits
__lib_ent_bottom:
		.word	0


		.section	.lib.stub,"wa",@progbits
__lib_stub_top:


		.section	.lib.stub.btm,"wa",@progbits
__lib_stub_bottom:
		.word	0


##############################################################################
###  moduleinfo

		.section	".xodata.sceModuleInfo","wa",@progbits
		#.section	".rodata.sceModuleInfo","wa",@progbits
		#.align	4
		
__moduleinfo:

#ifdef _NON_KERNEL
		.byte	0,0,1,1
#else
		.byte	0,0x10,1,1      # for kernel mode
#endif

		.ascii	"USERPROG"		#up to 28 char
		.align	5

		.word	_gp
		.word	__lib_ent_top
		.word	__lib_ent_bottom
		.word	__lib_stub_top
		.word	__lib_stub_bottom

##############################################################################
### entrytable

		.section	.rodata.entrytable,"a",@progbits
		#.section	".rodata.sceResident","a",@progbits
__entrytable:
		.word 0xD632ACDB
		.word 0xF01D73A7
		.word _start
		.word __moduleinfo
		.word 0


###############################################################################
### macro

	.macro	STUB_START	module,d1,d2

		.section	.rodata.stubmodulename
		.word	0
__stub_modulestr_\@:
		.asciz	"\module"
		.align	2

		.section	.lib.stub
		.word __stub_modulestr_\@
		.word \d1
		.word \d2
		.word __stub_idtable_\@
		.word __stub_text_\@

		.section	.rodata.stubidtable
__stub_idtable_\@:

		.section	.text.stub
__stub_text_\@:

	.endm


	.macro	STUB_END
	.endm


	.macro	STUB_FUNC	funcid,funcname

		.set push
		.set noreorder

		.section	.text.stub
		.weak	\funcname
\funcname:
		jr	$ra
		nop

		.section	.rodata.stubidtable
		.word	\funcid

		.set pop

	.endm

###############################################################################
### stub

	STUB_START "sceDisplay",0x40010000,0x00110005
	STUB_FUNC 0x0e20f177,sceDisplaySetMode
	STUB_FUNC 0xdea197d4,sceDisplayGetMode
	STUB_FUNC 0xdba6c4c4,sceDisplayGetFramePerSec
	STUB_FUNC 0x7ed59bc4,sceDisplay_Unkonow00
	STUB_FUNC 0xa544c486,sceDisplay_Unkonow01
	STUB_FUNC 0x289d82fe,sceDisplaySetFrameBuf
	STUB_FUNC 0xeeda2e54,sceDisplayGetFrameBuf
	STUB_FUNC 0xb4f378fa,sceDisplay_Unkonow02
	STUB_FUNC 0x31c4baa8,sceDisplay_Unkonow03
	STUB_FUNC 0x9c6eaad7,sceDisplayGetVcount
	STUB_FUNC 0x4d4e10ec,sceDisplayIsVblank
	STUB_FUNC 0x36cdfade,sceDisplayWaitVblank
	STUB_FUNC 0x8eb9ec49,sceDisplayWaitVblankCB
	STUB_FUNC 0x984c27e7,sceDisplayWaitVblankStart
	STUB_FUNC 0x46f186c3,sceDisplayWaitVblankStartCB
	STUB_FUNC 0x773dd3a3,sceDisplayGetCurrentHcount
	STUB_FUNC 0x210eab3a,sceDisplayGetAccumulatedHcount
	STUB_END 

	STUB_START	"sceGe_user",0x40010000,0x00090005
	STUB_FUNC	0xe47e40e4,sceGeEdramGetAddr
	STUB_FUNC	0xb77905ea,sceGeEdramSetAddrTranslation
	STUB_FUNC	0xab49e76a,sceGeListEnQueue
	STUB_FUNC	0x1c0d95a6,sceGeListEnQueueHead
	STUB_FUNC	0x03444eb4,sceGeListSync
	STUB_FUNC	0xb287bd61,sceGeDrawSync
	STUB_FUNC	0xe0d68148,sceGeListUpdateStallAddr
	STUB_FUNC	0xa4fc06a4,sceGeSetCallback
	STUB_FUNC	0x05db22ce,sceGeUnsetCallback

	STUB_START "sceCtrl",0x40010000,0x000a0005 
	STUB_FUNC 0x6a2774f3,sceCtrlSetSamplingCycle
	STUB_FUNC 0x02baad91,sceCtrlGetSamplingCycle
	STUB_FUNC 0x1f4011e6,sceCtrlSetSamplingMode
	STUB_FUNC 0xda6b76a1,sceCtrlGetSamplingMode
	STUB_FUNC 0x3a622550,sceCtrlPeekBufferPositive
	STUB_FUNC 0xc152080a,sceCtrlPeekBufferNegative
	STUB_FUNC 0x1f803938,sceCtrlReadBufferPositive
	STUB_FUNC 0x60b81f86,sceCtrlReadBufferNegative
	STUB_FUNC 0xb1d0e5cd,sceCtrlPeekLatch
	STUB_FUNC 0x0b588501,sceCtrlReadLatch
	STUB_END

	STUB_START	"IoFileMgrForUser",0x40010000,0x00250005
	STUB_FUNC	0xe95a012b, sceIoIoctlAsync
	STUB_FUNC	0x63632449, sceIoIoctl
	STUB_FUNC	0x54f5fb11, sceIoDevctl
	STUB_FUNC	0x89aa9906, sceIoOpenAsync
	STUB_FUNC	0x109f50bc, sceIoOpen
	STUB_FUNC	0xff5940b6, sceIoCloseAsync
	STUB_FUNC	0x810c4bc3, sceIoClose
	STUB_FUNC	0x71b19e77, sceIoLseekAsync
	STUB_FUNC	0x27eb27b8, sceIoLseek
	STUB_FUNC	0x68963324, sceIoLseek32
	STUB_FUNC	0x1B385D8F, sceIoLseek32Async
	STUB_FUNC	0xa0b5a7c2, sceIoReadAsync
	STUB_FUNC	0x6a638d83, sceIoRead
	STUB_FUNC	0x0facab19, sceIoWriteAsync
	STUB_FUNC	0x42ec03ac, sceIoWrite
	STUB_FUNC	0xb29ddf9c, sceIoDopen
	STUB_FUNC	0xe3eb004c, sceIoDread
	STUB_FUNC	0xeb092469, sceIoDclose
	STUB_FUNC	0xf27a9c51, sceIoRemove
	STUB_FUNC	0x06a70004, sceIoMkdir
	STUB_FUNC	0x1117c65f, sceIoRmdir
	STUB_FUNC	0x55f4717d, sceIoChdir
	STUB_FUNC	0xace946e8, sceIoGetStat
	STUB_FUNC	0x779103a0, sceIoRename
	STUB_FUNC	0xb2a628c1, sceIoAssign
	STUB_FUNC	0x6d08a871, sceIoUnassign
	STUB_FUNC	0xe8bc6571, sceIoCancel
	STUB_FUNC	0xb293727f, sceIoChangeAsyncPriority
	STUB_FUNC	0xcb05f8d6, sceIoGetAsyncStat
	STUB_FUNC	0x08bd7374, sceIoGetDevType
	STUB_FUNC	0x3251ea56, sceIoPollAsync
	STUB_FUNC	0xa12a0514, sceIoSetAsyncCallback
	STUB_FUNC	0xab96437f, sceIoSync
	STUB_FUNC	0x35dbd746, sceIoWaitAsyncCB
	STUB_FUNC	0xe23eec33, sceIoWaitAsync
	STUB_FUNC	0xb8a740f4, sceIoChStat
	STUB_FUNC	0xACE946E8,sceIoGetstat
	STUB_END

	STUB_START "sceSuspendForUser",0x40010000,0x00030005 
	STUB_FUNC 0xeadb1bd7,sceKernelPowerLock 
	STUB_FUNC 0x3aee7261,sceKernelPowerUnlock 
	STUB_FUNC 0x090ccb3f,sceKernelPowerTick 
	STUB_END 

	STUB_START   "LoadExecForUser",0x40010000,0x20005 
	STUB_FUNC   0x5572A5F,sceKernelExitGame 
	STUB_FUNC   0x4AC57943,sceKernelRegisterExitCallback 
	STUB_END 

	STUB_START "scePower",0x40010000,0x002e0005 
	STUB_FUNC 0x2b51fe2f,scePower_Unkonow00 
	STUB_FUNC 0x442bfbac,scePower_Unkonow01 
	STUB_FUNC 0xefd3c963,scePowerTick 
	STUB_FUNC 0xedc13fe5,scePowerGetIdleTimer 
	STUB_FUNC 0x7f30b3b1,scePowerIdleTimerEnable 
	STUB_FUNC 0x972ce941,scePowerIdleTimerDisable 
	STUB_FUNC 0x27f3292c,scePowerBatteryUpdateInfo 
	STUB_FUNC 0xe8e4e204,scePower_Unkonow02 
	STUB_FUNC 0xb999184c,scePower_Unkonow03 
	STUB_FUNC 0x87440f5e,scePowerIsPowerOnline 
	STUB_FUNC 0x0afd0d8b,scePowerIsBatteryExist 
	STUB_FUNC 0x1e490401,scePowerIsBatteryCharging 
	STUB_FUNC 0xb4432bc8,scePowerGetBatteryChargingStatus 
	STUB_FUNC 0xd3075926,scePowerIsLowBattery 
	STUB_FUNC 0x78a1a796,scePower_Unkonow04 
	STUB_FUNC 0x94f5a53f,scePower_Unkonow05 
	STUB_FUNC 0xfd18a0ff,scePower_Unkonow06 
	STUB_FUNC 0x2085d15d,scePowerGetBatteryLifePercent 
	STUB_FUNC 0x8efb3fa2,scePowerGetBatteryLifeTime 
	STUB_FUNC 0x28e12023,scePowerGetBatteryTemp 
	STUB_FUNC 0x862ae1a6,scePowerGetBatteryElec 
	STUB_FUNC 0x483ce86b,scePowerGetBatteryVolt 
	STUB_FUNC 0x23436a4a,scePower_Unkonow07 
	STUB_FUNC 0x0cd21b1f,scePower_Unkonow08 
	STUB_FUNC 0x165ce085,scePower_Unkonow09 
	STUB_FUNC 0xd6d016ef,scePowerLock 
	STUB_FUNC 0xca3d34c1,scePowerUnlock 
	STUB_FUNC 0xdb62c9cf,scePowerCancelRequest 
	STUB_FUNC 0x7fa406dd,scePowerIsRequest 
	STUB_FUNC 0x2b7c7cf4,scePowerRequestStandby 
	STUB_FUNC 0xac32c9cc,scePowerRequestSuspend 
	STUB_FUNC 0x2875994b,scePower_Unkonow10 
	STUB_FUNC 0x3951af53,scePower_Unkonow11 
	STUB_FUNC 0x0074ef9b,scePowerGetResumeCount 
	STUB_FUNC 0x04b7766e,scePowerRegisterCallback 
	STUB_FUNC 0xdfa8baf8,scePowerUnregisterCallback 
	STUB_FUNC 0xdb9d28dd,scePower_Unkonow12 
	STUB_FUNC 0x843fbf43,scePowerSetCpuClockFrequency 
	STUB_FUNC 0xb8d7b3fb,scePowerSetBusClockFrequency 
	STUB_FUNC 0xfee03a2f,scePowerGetCpuClockFrequency 
	STUB_FUNC 0x478fe6f5,scePowerGetBusClockFrequency 
	STUB_FUNC 0xfdb5bfe9,scePowerGetCpuClockFrequencyInt 
	STUB_FUNC 0xbd681969,scePowerGetBusClockFrequencyInt 
	STUB_FUNC 0xb1a52c83,scePowerGetCpuClockFrequencyFloat 
	STUB_FUNC 0x9badb3eb,scePowerGetBusClockFrequencyFloat 
	STUB_FUNC 0x737486f2,scePowerSetClockFrequency 
	STUB_END 

	STUB_START	"sceAudio",0x40010000,0x00090005
	STUB_FUNC	0x136CAF51,sceAudioOutputBlocking
	STUB_FUNC	0xE2D56B2D,sceAudioOutputPanned
	STUB_FUNC	0x13F592BC,sceAudioOutputPannedBlocking
	STUB_FUNC	0x5EC81C55,sceAudioChReserve
	STUB_FUNC	0x6FC46853,sceAudioChRelease
	STUB_FUNC	0xE9D97901,sceAudioGetChannelRestLen
	STUB_FUNC	0xCB2E439E,sceAudioSetChannelDataLen 
	STUB_FUNC	0x95FD0C2D,sceAudioChangeChannelConfig
	STUB_FUNC	0xB7E1D8E7,sceAudioChangeChannelVolume
	STUB_END

	STUB_START	"UtilsForUser",0x40010000,0x00040005
	STUB_FUNC	0x91E4F6A7,sceKernelLibcClock
	STUB_FUNC	0x27CC57F0,sceKernelLibcTime
	STUB_FUNC	0x71EC4271,sceKernelLibcGettimeofday
	STUB_FUNC	0x79D1C3FA,sceKernelDcacheWritebackAll
	STUB_FUNC	0xB435DEC5,sceKernelDcacheWritebackInvalidateAll
	STUB_FUNC	0x3EE30821,sceKernelDcacheWritebackRange
	STUB_END

	STUB_START   "ThreadManForUser",0x40010000,0x00170005
	STUB_FUNC   0x446D8DE6,sceKernelCreateThread
	STUB_FUNC   0xF475845D,sceKernelStartThread
	STUB_FUNC   0xAA73C935,sceKernelExitThread
	STUB_FUNC   0x9ACE131E,sceKernelSleepThread
	STUB_FUNC   0xD59EAD2F,sceKernelWakeupThread
	STUB_FUNC   0x55C20A00,sceKernelCreateEventFlag
	STUB_FUNC   0xEF9E4C70,sceKernelDeleteEventFlag
	STUB_FUNC   0x1FB15A32,sceKernelSetEventFlag
	STUB_FUNC   0x812346E4,sceKernelClearEventFlag
	STUB_FUNC   0x402FCF22,sceKernelWaitEventFlag
	STUB_FUNC   0x82826F70,sceKernelSleepThreadCB
	STUB_FUNC   0xE81CAF8F,sceKernelCreateCallback
	STUB_FUNC   0x278C0DF5,sceKernelWaitThreadEnd
	STUB_FUNC   0x9FA03CD3,sceKernelDeleteThread
	STUB_FUNC   0xCEADEB47,sceKernelDelayThread
	STUB_FUNC   0x809ce29b,sceKernelExitDeleteThread
	STUB_FUNC	0x7C0DC2A0,sceKernelCreateMsgPipe
	STUB_FUNC	0xF0B7DA1C,sceKernelDeleteMsgPipe
	STUB_FUNC	0xDF52098F,sceKernelTryReceiveMsgPipe
	STUB_FUNC	0x74829B76,sceKernelReceiveMsgPipe
	STUB_FUNC	0x876DBFAD,sceKernelSendMsgPipe
	STUB_FUNC	0x884C9F90,sceKernelTrySendMsgPipe
	STUB_FUNC	0x33BE4024,sceKernelReferMsgPipeStatus
	STUB_FUNC   0x9944F31F,sceKernelSuspendThread
	STUB_END

	STUB_START "sceDmac",0x40010000,0x00020005
	STUB_FUNC   0x617f3fe6,sceDmacMemcpy
	STUB_FUNC   0xd97f94d8,sceDmacTryMemcpy
	STUB_END 

	STUB_START "SysMemUserForUser",0x40000000,0x00050005
	STUB_FUNC 0x237DBD4F,sceKernelAllocPartitionMemory
	STUB_FUNC 0xB6D61D02,sceKernelFreePartitionMemory
	STUB_FUNC 0x9D9A5BA1,sceKernelGetBlockHeadAddr
	STUB_FUNC 0xa291f107,sceKernelMaxFreeMemSize
	STUB_FUNC 0xf919f628,sceKernelTotalFreeMemSize
	STUB_END

	STUB_START "StdioForUser",0x40010000,0x00030005 
	STUB_FUNC 0x172D316E,sceKernelStdin 
	STUB_FUNC 0xA6BAB2E9,sceKernelStdout 
	STUB_FUNC 0xF78BA90A,sceKernelStderr 
	STUB_END

	STUB_START "sceRtc",0x40010000,0x00220005 
	STUB_FUNC 0xc41c2853,sceRtcGetTickResolution 
	STUB_FUNC 0x3f7ad767,sceRtcGetCurrentTick 
	STUB_FUNC 0x4cfa57b0,sceRtcGetCurrentClock 
	STUB_FUNC 0xe7c27d1b,sceRtcGetCurrentClockLocalTime 
	STUB_FUNC 0x34885e0d,sceRtcConvertUtcToLocalTime 
	STUB_FUNC 0x779242a2,sceRtcConvertLocalTimeToUTC 
	STUB_FUNC 0x42307a17,sceRtcIsLeapYear 
	STUB_FUNC 0x05ef322c,sceRtcGetDaysInMonth 
	STUB_FUNC 0x57726bc1,sceRtcGetDayOfWeek 
	STUB_FUNC 0x4b1b5e82,sceRtcCheckValid 
	STUB_FUNC 0x3a807cc8,sceRtcSetTime_t 
	STUB_FUNC 0x27c4594c,sceRtcGetTime_t 
	STUB_FUNC 0xf006f264,sceRtcSetDosTime 
	STUB_FUNC 0x36075567,sceRtcGetDosTime 
	STUB_FUNC 0x7ace4c04,sceRtcSetWin32FileTime 
	STUB_FUNC 0xcf561893,sceRtcGetWin32FileTime 
	STUB_FUNC 0x7ed29e40,sceRtcSetTick 
	STUB_FUNC 0x6ff40acc,sceRtcGetTick 
	STUB_FUNC 0x9ed0ae87,sceRtcCompareTick 
	STUB_FUNC 0x44f45e05,sceRtcTickAddTicks 
	STUB_FUNC 0x26d25a5d,sceRtcTickAddMicroseconds 
	STUB_FUNC 0xf2a4afe5,sceRtcTickAddSeconds 
	STUB_FUNC 0xe6605bca,sceRtcTickAddMinutes 
	STUB_FUNC 0x26d7a24a,sceRtcTickAddHours 
	STUB_FUNC 0xe51b4b7a,sceRtcTickAddDays 
	STUB_FUNC 0xcf3a2ca8,sceRtcTickAddWeeks 
	STUB_FUNC 0xdbf74f1b,sceRtcTickAddMonths 
	STUB_FUNC 0x42842c77,sceRtcTickAddYears 
	STUB_FUNC 0xc663b3b9,sceRtcFormatRFC2822 
	STUB_FUNC 0x7de6711b,sceRtcFormatRFC2822LocalTime 
	STUB_FUNC 0x0498fb3c,sceRtcFormatRFC3339 
	STUB_FUNC 0x27f98543,sceRtcFormatRFC3339LocalTime 
	STUB_FUNC 0xdfbc5f16,sceRtcParseDateTime 
	STUB_FUNC 0x28e1e988,sceRtcParseRFC3339 
	STUB_END 

	STUB_START "InterruptManager",0x40010000,0x00090005
	STUB_FUNC 0xCA04A2B9,sceKernelRegisterSubIntrHandler
	STUB_FUNC 0xD61E6961,sceKernelReleaseSubIntrHandler
	STUB_FUNC 0xFB8E22EC,sceKernelEnableSubIntr
	STUB_FUNC 0x8A389411,sceKernelDisableSubIntr
	STUB_FUNC 0x5CB5A78B,sceKernelSuspendSubIntr
	STUB_FUNC 0x7860E0DC,sceKernelResumeSubIntr
	STUB_FUNC 0xFC4374B8,sceKernelIsSubInterruptOccurred
	STUB_FUNC 0xD2E8363F,QueryIntrHandlerInfo
	STUB_FUNC 0xEEE43F47,sceKernelRegisterUserSpaceIntrStack
	STUB_END

	STUB_START "ModuleMgrForUser",0x40010000,0x000A0005
	STUB_FUNC 0xb7f46618,sceKernelLoadModuleByID
	STUB_FUNC 0x977de386,sceKernelLoadModule
	STUB_FUNC 0x710f61b5,sceKernelLoadModuleMs
	STUB_FUNC 0xf9275d98,sceKernelLoadModuleBufferUsbWlan
	STUB_FUNC 0x50f0c1ec,sceKernelStartModule
	STUB_FUNC 0xd1ff982a,sceKernelStopModule
	STUB_FUNC 0x2e0911aa,sceKernelUnloadModule
	STUB_FUNC 0xd675ebb8,sceKernelSelfStopUnloadModule
	STUB_FUNC 0xcc1d3699,sceKernelStopUnloadSelfModule
	STUB_FUNC 0x748cbed9,sceKernelQueryModuleInfo
	STUB_END

#ifndef _NON_KERNEL
##############################################################
### for Kernel

	STUB_START "LoadCoreForKernel",0x40010000,0x00010005
	STUB_FUNC  0xcce4a157,sceKernelFindModuleByUID
	STUB_END

	STUB_START "UtilsForKernel",0x40010000,0x00010005
	STUB_FUNC  0x920f104a,sceKernelIcacheInvalidateAll
	STUB_END

#	STUB_START "ThreadManForKernel",0x40010000,0x00060005 
#	STUB_FUNC 0x446d8de6,_sceKernelCreateThread
#	STUB_FUNC 0x9fa03cd3,_sceKernelDeleteThread
#	STUB_FUNC 0xf475845d,_sceKernelStartThread
#	STUB_FUNC 0xaa73c935,_sceKernelExitThread
#	STUB_FUNC 0x809ce29b,_sceKernelExitDeleteThread
#	STUB_FUNC 0x9ace131e,_sceKernelSleepThread
#	STUB_END 

#ifdef _USE_ME
##############################################################
### for ME

	STUB_START "sceSysreg_driver",0x40010000,0x00040005
	STUB_FUNC  0x44F6CDA7,sceSysregMeBusClockEnable
	STUB_FUNC  0x158AD4FC,sceSysregMeBusClockDisable
	STUB_FUNC  0xDE59DACB,sceSysregMeResetEnable
	STUB_FUNC  0x2DB0EB28,sceSysregMeResetDisable
	STUB_END
##############################################################
#endif // _USE_ME

##############################################################
### for Adhoc

		STUB_START "sceNet",0x90000,0x00080005 
		STUB_FUNC 0x39af39a6,sceNetInit 
		STUB_FUNC 0x281928a9,sceNetTerm 
		STUB_FUNC 0x50647530,sceNetFreeThreadinfo 
		STUB_FUNC 0xad6844c6,sceNetThreadAbort 
		STUB_FUNC 0x89360950,sceNetEtherNtostr 
		STUB_FUNC 0xd27961c9,sceNetEtherStrton 
		STUB_FUNC 0x0bf0a3ae,sceNetGetLocalEtherAddr 
		STUB_FUNC 0xcc393e48,sceNetGetMallocStat 
		STUB_END 

		STUB_START "sceNetInet",0x90000,0x001c0005 
		STUB_FUNC 0x17943399,sceNetInetInit 
		STUB_FUNC 0xa9ed66b9,sceNetInetTerm 
		STUB_FUNC 0xdb094e1b,sceNetInetAccept 
		STUB_FUNC 0x1a33f9ae,sceNetInetBind 
		STUB_FUNC 0x8d7284ea,sceNetInetClose 
		STUB_FUNC 0x805502dd,sceNetInetCloseWithRST 
		STUB_FUNC 0x410b34aa,sceNetInetConnect 
		STUB_FUNC 0xe247b6d6,sceNetInetGetpeername 
		STUB_FUNC 0x162e6fd5,sceNetInetGetsockname 
		STUB_FUNC 0x4a114c7c,sceNetInetGetsockopt 
		STUB_FUNC 0xd10a1a7a,sceNetInetListen 
		STUB_FUNC 0xfaabb1dd,sceNetInetPoll 
		STUB_FUNC 0xcda85c99,sceNetInetRecv 
		STUB_FUNC 0xc91142e4,sceNetInetRecvfrom 
		STUB_FUNC 0xeece61d2,sceNetInetRecvmsg 
		STUB_FUNC 0x5be8d595,sceNetInetSelect 
		STUB_FUNC 0x7aa671bc,sceNetInetSend 
		STUB_FUNC 0x05038fc7,sceNetInetSendto 
		STUB_FUNC 0x774e36f4,sceNetInetSendmsg 
		STUB_FUNC 0x2fe71fe7,sceNetInetSetsockopt 
		STUB_FUNC 0x4cfe4e56,sceNetInetShutdown 
		STUB_FUNC 0x8b7b220f,sceNetInetSocket 
		STUB_FUNC 0x80a21abd,sceNetInetSocketAbort 
		STUB_FUNC 0xfbabe411,sceNetInetGetErrno 
		STUB_FUNC 0xb75d5b0a,sceNetInetInetAddr 
		STUB_FUNC 0x1bdf5d13,sceNetInetInetAton 
		STUB_FUNC 0xd0792666,sceNetInetInetNtop 
		STUB_FUNC 0xe30b8c19,sceNetInetInetPton 
		STUB_END 

		STUB_START "sceNetResolver",0x90000,0x00070005 
		STUB_FUNC 0xf3370e61,sceNetResolverInit  
		STUB_FUNC 0x6138194a,sceNetResolverTerm  
		STUB_FUNC 0x244172af,sceNetResolverCreate  
		STUB_FUNC 0x94523e09,sceNetResolverDelete  
		STUB_FUNC 0x224c5f44,sceNetResolverStartNtoA  
		STUB_FUNC 0x629e2fb7,sceNetResolverStartAtoN  
		STUB_FUNC 0x808f6063,sceNetResolverStop 
		STUB_END 

		STUB_START "sceNetAdhoc",0x90000,0x00190005 
		STUB_FUNC 0xe1d621d7,sceNetAdhocInit  
		STUB_FUNC 0xa62c6f57,sceNetAdhocTerm  
		STUB_FUNC 0x7a662d6b,sceNetAdhocPollSocket  
		STUB_FUNC 0x73bfd52d,sceNetAdhocSetSocketAlert  
		STUB_FUNC 0x4d2ce199,sceNetAdhocGetSocketAlert  
		STUB_FUNC 0x6f92741b,sceNetAdhocPdpCreate  
		STUB_FUNC 0xabed3790,sceNetAdhocPdpSend  
		STUB_FUNC 0xdfe53e03,sceNetAdhocPdpRecv  
		STUB_FUNC 0x7f27bb5e,sceNetAdhocPdpDelete  
		STUB_FUNC 0xc7c1fc57,sceNetAdhocGetPdpStat  
		STUB_FUNC 0x877f6d66,sceNetAdhocPtpOpen  
		STUB_FUNC 0xfc6fc07b,sceNetAdhocPtpConnect  
		STUB_FUNC 0xe08bdac1,sceNetAdhocPtpListen  
		STUB_FUNC 0x9df81198,sceNetAdhocPtpAccept  
		STUB_FUNC 0x4da4c788,sceNetAdhocPtpSend  
		STUB_FUNC 0x8bea2b3e,sceNetAdhocPtpRecv  
		STUB_FUNC 0x9ac2eeac,sceNetAdhocPtpFlush  
		STUB_FUNC 0x157e6225,sceNetAdhocPtpClose  
		STUB_FUNC 0xb9685118,sceNetAdhocGetPtpStat  
		STUB_FUNC 0x7f75c338,sceNetAdhocGameModeCreateMaster  
		STUB_FUNC 0x3278ab0c,sceNetAdhocGameModeCreateReplica  
		STUB_FUNC 0x98c204c8,sceNetAdhocGameModeUpdateMaster  
		STUB_FUNC 0xfa324b4e,sceNetAdhocGameModeUpdateReplica  
		STUB_FUNC 0xa0229362,sceNetAdhocGameModeDeleteMaster  
		STUB_FUNC 0x0b2228e9,sceNetAdhocGameModeDeleteReplica 
		STUB_END 

		STUB_START "sceNetAdhocctl",0x90000,0x00140005 
		STUB_FUNC 0xe26f226e,sceNetAdhocctlInit  
		STUB_FUNC 0x9d689e13,sceNetAdhocctlTerm  
		STUB_FUNC 0x0ad043ed,sceNetAdhocctlConnect  
		STUB_FUNC 0xec0635c1,sceNetAdhocctlCreate  
		STUB_FUNC 0x5e7f79c9,sceNetAdhocctlJoin  
		STUB_FUNC 0x08fff7a0,sceNetAdhocctlScan  
		STUB_FUNC 0x34401d65,sceNetAdhocctlDisconnect  
		STUB_FUNC 0x20b317a0,sceNetAdhocctlAddHandler  
		STUB_FUNC 0x6402490b,sceNetAdhocctlDelHandler  
		STUB_FUNC 0x75ecd386,sceNetAdhocctlGetState  
		STUB_FUNC 0x362cbe8f,sceNetAdhocctlGetAdhocId  
		STUB_FUNC 0xe162cb14,sceNetAdhocctlGetPeerList  
		STUB_FUNC 0x99560abe,sceNetAdhocctlGetAddrByName  
		STUB_FUNC 0x8916c003,sceNetAdhocctlGetNameByAddr  
		STUB_FUNC 0xded9d28e,sceNetAdhocctlGetParameter  
		STUB_FUNC 0x81aee1be,sceNetAdhocctlGetScanInfo  
		STUB_FUNC 0xa5c055ce,sceNetAdhocctlCreateEnterGameMode  
		STUB_FUNC 0x1ff89745,sceNetAdhocctlJoinEnterGameMode  
		STUB_FUNC 0xcf8e084d,sceNetAdhocctlExitGameMode  
		STUB_FUNC 0x5a014ce0,sceNetAdhocctlGetGameModeInfo 
		STUB_END 

		STUB_START "sceNetAdhocMatching",0x90000,0x000c0005 
		STUB_FUNC 0x2a2a1e07,sceNetAdhocMatchingInit 
		STUB_FUNC 0x7945ecda,sceNetAdhocMatchingTerm 
		STUB_FUNC 0xca5eda6f,sceNetAdhocMatchingCreate 
		STUB_FUNC 0x93ef3843,sceNetAdhocMatchingStart 
		STUB_FUNC 0x32b156b3,sceNetAdhocMatchingStop 
		STUB_FUNC 0xf16eaf4f,sceNetAdhocMatchingDelete 
		STUB_FUNC 0x5e3d4b79,sceNetAdhocMatchingSelectTarget 
		STUB_FUNC 0xea3c6108,sceNetAdhocMatchingCancelTarget 
		STUB_FUNC 0xb58e61b7,sceNetAdhocMatchingSetHelloOpt 
		STUB_FUNC 0xb5d96c2a,sceNetAdhocMatchingGetHelloOpt 
		STUB_FUNC 0xc58bcd9e,sceNetAdhocMatchingGetMembers 
		STUB_FUNC 0x40f8f435,sceNetAdhocMatchingGetPoolMaxAlloc 
		STUB_END 

		STUB_START "sceNetApctl",0x90000,0x00080005 
		STUB_FUNC 0xe2f91f9b,sceNetApctlInit  
		STUB_FUNC 0xb3edd0ec,sceNetApctlTerm  
		STUB_FUNC 0x2befdf23,sceNetApctlGetInfo  
		STUB_FUNC 0x8abadd51,sceNetApctlAddHandler  
		STUB_FUNC 0x5963991b,sceNetApctlDelHandler  
		STUB_FUNC 0xcfb957c6,sceNetApctlConnect  
		STUB_FUNC 0x24fe91a1,sceNetApctlDisconnect  
		STUB_FUNC 0x5deac81b,sceNetApctlGetState 
		STUB_END 

		STUB_START "sceWlanDrv",0x40010000,0x00040005 
		STUB_FUNC 0x93440b11,sceWlanDevIsPowerOn 
		STUB_FUNC 0x93440b11,sceWlanGPBindRegError 
		STUB_FUNC 0xd7763699,sceWlanGetSwitchState 
		STUB_FUNC 0x0c622081,sceWlanGetEtherAddr 
		STUB_END 

##############################################################
#endif  //_NON_KERNEL

	.text

	.end _start

##############################################################
