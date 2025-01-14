//***************************************************************************
//
//  Copyright (c) 2001 - 2006 Intel Corporation
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
//***************************************************************************

/**
	@file	CIFXAuthorPointSetResource.h

			This module defines the CIFXAuthorPointSetResource component.
*/

#ifndef CIFXAuthorPointSetResource_H
#define CIFXAuthorPointSetResource_H

#include "IFXAuthorPointSetResource.h"
#include "IFXAuthorPointSet.h"
#include "IFXMesh.h"
#include "IFXMeshMap.h"
#include "IFXNeighborResControllerIntfc.h"
#include "CIFXModifier.h"
#include "IFXBoundSphereDataElement.h"
#include "IFXUpdatesGroup.h"
#include "IFXSkeleton.h"
#include "IFXAutoRelease.h"

struct IFXPointSetSize
{
	IFXPointSetSize() : m_vertexAttributes() 
	{
		m_numVertices = U32(-1);
	}

	IFXVertexAttributes m_vertexAttributes;
	U32 m_numVertices;
};


class CIFXAuthorPointSetResource :	public CIFXModifier,
							virtual public  IFXAuthorPointSetResource
{
public:
	/// @name IFXUnknown
	//@{
	U32 IFXAPI        AddRef();
	U32 IFXAPI        Release();
	IFXRESULT IFXAPI  QueryInterface( IFXREFIID riid, void **ppv );
	//@}

	/// @name IFXMarker
	//@{
	IFXRESULT   IFXAPI   SetSceneGraph( IFXSceneGraph* pInSceneGraph );
	//@}
	
	/// @name IFXMarkerX
	//@{
	void IFXAPI  GetEncoderX( IFXEncoderX*& rpEncoderX );
	//@}

	/// @name IFXModifier
	//@{
	IFXRESULT IFXAPI  GetOutputs( 
							IFXGUID**& rpOutOutputs, 
							U32&       rOutNumberOfOutputs,
							U32*&	   rpOutOutputDepAttrs );
	
	IFXRESULT IFXAPI  GetDependencies(    
							IFXGUID*   pInOutputDID,
							IFXGUID**& rppOutInputDependencies,
							U32&       rOutNumberInputDependencies,
							IFXGUID**& rppOutOutputDependencies,
							U32&       rOutNumberOfOutputDependencies,
							U32*&	   rpOutOutputDepAttrs );

	IFXRESULT IFXAPI  GenerateOutput (
							U32    inOutputDataElementIndex, 
	                        void*& rpOutData, BOOL& rNeedRelease ); 
	
	IFXRESULT IFXAPI  SetDataPacket ( 
							IFXModifierDataPacket* pInInputDataPacket, 
							IFXModifierDataPacket* pInDataPacket );
	
	IFXRESULT IFXAPI  Notify ( 
							IFXModifierMessage eInMessage, 
							void*               pMessageContext )
	{ 
		return IFX_OK; 
	}
	//@}

	// IFXAuthorPointSetResource
	const IFXVector4& IFXAPI GetBoundingSphere() 
	{ 
		return m_pBoundSphereDataElement->Bound(); 
	}

	IFXRESULT IFXAPI   SetBoundingSphere(const IFXVector4& rBoundingSphere) 
	{ 
		m_pBoundSphereDataElement->Bound() = rBoundingSphere; 
		return IFX_OK; 
	}

	const IFXMatrix4x4& IFXAPI GetTransform() 
	{ 
		return m_transform[0]; 
	}

	IFXRESULT  IFXAPI 	 SetTransform(const IFXMatrix4x4& rTransform);
	IFXRESULT  IFXAPI 	 InvalidateTransform();

	IFXRESULT  IFXAPI 	 GetMeshGroup(IFXMeshGroup**);

	virtual IFXRESULT IFXAPI  BuildDataBlockQueue();
	
	virtual void IFXAPI  GetDataBlockQueueX(IFXDataBlockQueueX*& rpDataBlockQueueX);

	IFXRESULT  IFXAPI 	 GetNeighborMesh(IFXNeighborMesh** out_ppNeighborMesh);
	IFXRESULT  IFXAPI 	 GetUpdatesGroup(IFXUpdatesGroup** out_ppUpdatesGroup);

	IFXRESULT  IFXAPI 	 GetAuthorPointSet(IFXAuthorPointSet*& rpAuthorPointSet) ;
	IFXRESULT  IFXAPI 	 SetAuthorPointSet(IFXAuthorPointSet* pAuthorPointSet) ;
	
	IFXNeighborResControllerInterface* GetNeighborResController();
	
	// Bones support
	IFXSkeleton* IFXAPI  GetBones( void ) { return m_pBones; }
	IFXRESULT  IFXAPI 	 SetBones( IFXSkeleton *pBonesGen ) 
	{
		IFXRESULT result = IFX_OK;

		if( pBonesGen )
		{
			m_pBones = pBonesGen; 
			m_pBones->AddRef(); 
		}
		else
			result = IFX_E_INVALID_POINTER;

		return result; 
	}

	IFXRESULT  IFXAPI 	 Transfer();
 
 	IFXRESULT  IFXAPI 	 GetAuthorMeshMap(IFXMeshMap **ppAuthorPointSethMap);
 	IFXRESULT  IFXAPI 	 SetAuthorMeshMap(IFXMeshMap *pAuthorPointSethMap);
 
 	IFXRESULT  IFXAPI 	 GetRenderMeshMap(IFXMeshMap **ppRenderPointSetMap);
 	IFXRESULT  IFXAPI 	 SetRenderMeshMap(IFXMeshMap *pRenderPointSetMap);

	class VertexDescriptor 
	{
	public:
		VertexDescriptor();
		~VertexDescriptor();
		VertexDescriptor* Clone();
		BOOL EqualAttributes(VertexDescriptor*);
	
		/* 
			this will go unused for static compile, keep it since this struct is 
			nice 32 byte cache Point friendly.
		*/
		IFXVector3 Normal;

		U32 AuthorIndex;
		U32	IFXIndex;
		U32 NumAttributes;
		U32* pAttributes;    // array of indicies for each attribute
		VertexDescriptor* pNext;
	};

private:
	IFXRESULT BuildMeshGroup();
    IFXRESULT BuildNeighborMesh();
    IFXRESULT BuildNeighborResController();
    void ClearMeshGroup();

	IFXRESULT cpl_EvaluatePointSetSizes(); 
	IFXRESULT cpl_AllocateOutput();
	IFXRESULT cpl_AllocatePointSetMap();
	IFXRESULT cpl_AllocateVertexHashAndVertexDescQuery();
	IFXRESULT cpl_Compile();
	IFXRESULT cpl_FindAndBuildVertex(U32 authorPointIndex, U32 mat, U32* pOutIFXIndex);

	class VertexHash
	{
	public:
		VertexHash();
		~VertexHash();
		IFXRESULT Allocate(U32 numAuthorVerts);
		void Insert(VertexDescriptor*);
		VertexDescriptor* FindExactMatch(VertexDescriptor*); // used by static compile.
	private:
		VertexDescriptor** m_ppArray;
		U32 m_Size;
	};

	CIFXAuthorPointSetResource();
	virtual ~CIFXAuthorPointSetResource();
	friend IFXRESULT IFXAPI_CALLTYPE CIFXAuthorPointSetResource_Factory( 
											IFXREFIID interfaceId, void** ppInterface );

	// IFXUnknown 
	U32 m_uRefCount;

	// IFXAuthorPointSetResource
	U32 m_uTransformDataElementIndex;
	U32 m_uRenderableGroupDataElementIndex;
	U32 m_uNeighborResControllerDataElementIndex;
	U32 m_uNeighborMeshDataElementIndex;
	U32 m_uBoundSphereDataElementIndex;
	U32 m_uBonesManagerDataElementIndex;

	IFXAuthorPointSet*	m_pAuthorPointSet;	
	
	// these members will be used at convertion of AuthorPointSet to renderable 
	// PointSets they are direct access to AuthorPointSet content

	IFXVector3*	m_pPositions;
	IFXVector3*	m_pNormals;
	IFXVector4*	m_pTextures;
	IFXVector4*	m_pSpeculars;
	IFXVector4*	m_pDiffuses;
	U32*		m_pPointMaterials;

	U32*		m_pPositionPoints; 
	U32*		m_pNormalPoints;
	U32*		m_pTexturePoints[IFX_MAX_TEXUNITS];
	U32*		m_pSpecularPoints;
	U32* 		m_pDiffusePoints;
	
	// num of Materials(Shaders) = num PointSets in Group = num of PointSetSizes
	U32							m_numMaterials;  
	U32							m_numPointSetPoints;
	IFXPointSetSize*			m_pPointSetSizes;
	IFXAuthorMaterial*			m_pMaterials;
	BOOL						m_bMeshGroupDirty;
	
	IFXMeshGroup*		    	m_pMeshGroup;
	IFXNeighborMesh*            m_pNeighborMesh;
	IFXUpdatesGroup*            m_pUpdatesGroup;

    BOOL						m_bBuiltNeighborMesh;
    IFXNeighborResControllerInterface* m_pNeighborResController;

	VertexDescriptor*			m_pQV;
	VertexHash*					m_pVertexHash;

	// These objects are used to map position, normales and other indexes 
	// from AuthorPointSet to it's renderable representation IFXMeshGroup.
	// So we are not creating new PointSet spefic classes, but using general IFXMeshMap
	IFXMeshMap*					m_pAuthorPointSetMap;
 	IFXMeshMap*					m_pRenderPointSetMap;
	
	// pointer to array of iters, one for each material in output ifx Point set.
	IFXVertexIter* m_pIteratorCache;

	// fast access to maps for each attribute
	IFXVertexMap *m_pPointMap;
	IFXVertexMap *m_pPositionMap;
	IFXVertexMap *m_pNormalMap;
	IFXVertexMap *m_pTextureMap;
	IFXVertexMap *m_pDiffuseMap;
	IFXVertexMap *m_pSpecularMap;

	IFXArray<IFXMatrix4x4> m_transform;
	IFXBoundSphereDataElement*	m_pBoundSphereDataElement;

	IFXDECLAREMEMBER(IFXDataBlockQueueX,m_pDataBlockQueueX);

	IFXSkeleton* m_pBones;
};

IFXINLINE CIFXAuthorPointSetResource::VertexDescriptor::VertexDescriptor()
{
	NumAttributes = 0;
	pAttributes = NULL;
	pNext = NULL;
	AuthorIndex = 0;
	IFXIndex = 0;
}

IFXINLINE CIFXAuthorPointSetResource::VertexDescriptor::~VertexDescriptor()
{
	IFXDELETE_ARRAY(pAttributes);
	if(pNext) delete pNext;   // delete the chain
}

IFXINLINE CIFXAuthorPointSetResource::VertexDescriptor* 
CIFXAuthorPointSetResource::VertexDescriptor::Clone()
{
	VertexDescriptor *pNew = new VertexDescriptor;
	if(pNew == NULL) return pNew;

	*pNew = *this;
	U32 *pAttributesNew = new U32[this->NumAttributes];
	if(pAttributesNew == NULL) 
	{ 
		delete pNew;
		pNew = NULL;
		return pNew;
	}
	pNew->pAttributes = pAttributesNew;

	U32 i;
	for(i=0; i < this->NumAttributes;i++)
		pNew->pAttributes[i] = this->pAttributes[i];

	return pNew;
}

IFXINLINE BOOL 
CIFXAuthorPointSetResource::VertexDescriptor::EqualAttributes(VertexDescriptor* pV)
{
	BOOL result = TRUE;
	U32 i;
	for(i = 0; i < NumAttributes; i++)
	{
		if(pV->pAttributes[i] != this->pAttributes[i])
		{
			result = FALSE;
			break;
		}
	}

	return result;
}

CIFXAuthorPointSetResource::VertexHash::VertexHash()
{
	m_ppArray = NULL;
	m_Size = 0;
}

CIFXAuthorPointSetResource::VertexHash::~VertexHash()
{
	U32 i;
	for(i=0; i<m_Size; i++)
		IFXDELETE(m_ppArray[i]);
	m_Size = 0;
	IFXDELETE_ARRAY(m_ppArray);
}

IFXRESULT CIFXAuthorPointSetResource::VertexHash::Allocate(U32 numAuthorPositions)
{
	m_ppArray = new VertexDescriptor*[numAuthorPositions];
	if(m_ppArray) 
	{
		memset(m_ppArray,0,numAuthorPositions*sizeof(VertexDescriptor*));
		m_Size = numAuthorPositions;
		return IFX_OK;
	}
	else
		return IFX_E_OUT_OF_MEMORY;
}

IFXINLINE void CIFXAuthorPointSetResource::VertexHash::Insert(VertexDescriptor* pVD)
{
	if( NULL != pVD )
	{
		if( pVD->AuthorIndex < m_Size )
		{
			pVD->pNext = m_ppArray[pVD->AuthorIndex];
			m_ppArray[pVD->AuthorIndex] = pVD;
		}
	}
}

/** 
	find best match on normal and exact match on all required attributes
	such as position, material, texture coord, colors.  
	This is used by the streaming CLOD compile.

	@todo CIFXAuthorPointSetResource does not support CLOD. Consider to remove.
*/
IFXINLINE CIFXAuthorPointSetResource::VertexDescriptor* 
CIFXAuthorPointSetResource::VertexHash::FindExactMatch(VertexDescriptor* pVD)
{
	VertexDescriptor *pCurrentVD;
	
	pCurrentVD = m_ppArray[pVD->AuthorIndex];
	
	while(pCurrentVD)
		if( pVD->EqualAttributes(pCurrentVD) )  // match exactly all attributes
			return pCurrentVD;
		else
			pCurrentVD = pCurrentVD->pNext;

	return pCurrentVD; 
}

#endif
