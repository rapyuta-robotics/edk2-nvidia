/** @file
  Configuration Manager Data Driver private structures for SMBIOS tables.

  SPDX-FileCopyrightText: Copyright (c) 2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __SMBIOS_PARSER_PRIVATE_H__
#define __SMBIOS_PARSER_PRIVATE_H__

#include <Library/FruLib.h>
#include <Library/NvCmObjectDescUtility.h>

#define MAX_SMBIOS_TABLE_TYPES_SUPPORTED       64
#define MAX_TYPE2_COUNT                        10
#define MAX_TYPE3_COUNT                        100
#define MAX_TYPE3_CONTAINED_ELEMENT_COUNT      100
#define MAX_TYPE41_COUNT                       100
#define TYPE41_DEVICE_NOT_PRESENT              0xFFFFFFFF
#define TYPE41_ONBOARD_DEVICE_ENABLED          0x80
#define MAX_TPM_VERSION_LEN                    14
#define MAX_FIRMWARE_INVENTORY_FMP_DESC_COUNT  100

//
// CM SMBIOS record population struct
//
typedef struct {
  UINT8              FruDeviceId;
  CM_OBJECT_TOKEN    ChassisCmToken;
} CM_ENCLOSURE_BASEBOARD_INFO;

/** This structure contains data used by SMBIOS CM object creators */
typedef struct CmSmbiosPrivateData {
  /// List of SMBIOS Tables that will be installed (EStdObjSmbiosTableList)
  CM_STD_OBJ_SMBIOS_TABLE_INFO    CmSmbiosTableList[MAX_SMBIOS_TABLE_TYPES_SUPPORTED];

  /// Number of SMBIOS Tables that will be installed
  UINTN                           CmSmbiosTableCount;

  /// Pointer to device tree
  VOID                            *DtbBase;

  /// Device tree size
  UINTN                           DtbSize;

  /// Offset to '/firmware/smbios' node
  INTN                            DtbSmbiosOffset;

  /// Pointer to FRU info array
  FRU_DEVICE_INFO                 **FruInfo;

  /// Number of FRUs in FRU info array
  UINT8                           FruCount;

  /// Enclosure and baseboard binding info
  struct {
    CM_ENCLOSURE_BASEBOARD_INFO    *Info;
    UINT8                          Count;
  } EnclosureBaseboardBinding;
} CM_SMBIOS_PRIVATE_DATA;

typedef EFI_STATUS
(EFIAPI *CM_INSTALL_SMBIOS_RECORD)(
  IN  CONST HW_INFO_PARSER_HANDLE  ParserHandle,
  IN OUT CM_SMBIOS_PRIVATE_DATA  *Private
  );

//
// CM SMBIOS record population struct
//
typedef struct {
  SMBIOS_TYPE                 Type;
  CM_INSTALL_SMBIOS_RECORD    Function;
} CM_SMBIOS_RECORD_POPULATION;

/**
  Find FRU by FRU description

  @param[in] Private        Pointer to the private data of SMBIOS creators
  @param[in] FruDescPattern FRU description pattern to look for

  @return A pointer to the FRU record if found, or NULL if not found.

**/
FRU_DEVICE_INFO *
FindFruByDescription (
  IN  CM_SMBIOS_PRIVATE_DATA  *Private,
  IN  CHAR8                   *FruDescPattern
  );

/**
  Find and get FRU extra string that has a certain prefix

  @param[in] FruExtra  Pointer to the array of FRU (chassis/board/product) extra
  @param[in] Prefix    FRU extra prefix to search for

  @return A pointer to an allocated string

**/
CHAR8 *
GetFruExtraStr (
  IN CHAR8        **FruExtra,
  IN CONST CHAR8  *Prefix
  );

/**
  Allocate and copy string

  @param[in] String     String to be copied

  @return A pointer to the copied string if succeeds, or NULL if fails.

**/
CHAR8 *
AllocateCopyString (
  IN  CHAR8  *String
  );

/**
Pci slot info and class code association structure
**/
typedef struct {
  UINT8    PciClass;
  CHAR8    *SlotDescription;
} PCI_SLOT_ASSOCIATION;

#endif
