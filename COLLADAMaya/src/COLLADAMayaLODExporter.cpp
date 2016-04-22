/*
    Copyright (c) 2008-2009 NetAllied Systems GmbH

	This file is part of COLLADAMaya.

    Portions of the code are:
    Copyright (c) 2005-2007 Feeling Software Inc.
    Copyright (c) 2005-2007 Sony Computer Entertainment America
    Copyright (c) 2004-2005 Alias Systems Corp.
	
    Licensed under the MIT Open Source License, 
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/

#include "COLLADAMayaStableHeaders.h"
#include "COLLADAMayaAttributeParser.h"
#include "COLLADAMayaSceneGraph.h"
#include "COLLADAMayaEffectExporter.h"
#include "COLLADAMayaLODExporter.h"
#include "COLLADAMayaExportOptions.h"
#include "COLLADAMayaAnimationExporter.h"
#include "COLLADAMayaVisualSceneExporter.h"

#include <maya/MFnAttribute.h>



namespace COLLADAMaya
{

    //---------------------------------------------------------------
	LODExporter::LODExporter(
        COLLADASW::StreamWriter* streamWriter,
        DocumentExporter* documentExporter )
		: COLLADASW::LibraryNodes(streamWriter)
    , mDocumentExporter ( documentExporter )
    {}


    //---------------------------------------------------------------
	void LODExporter::exportLODs(VisualSceneExporter* mVisualSceneExporter)
    {
		if (!ExportOptions::exportLOD())
			return;

		// Get the list with the transform nodes.
		SceneGraph* sceneGraph = mDocumentExporter->getSceneGraph();
		SceneElementsList* exportNodesTree = sceneGraph->getExportNodesTree();

		// Export all/selected DAG nodes
		size_t length = exportNodesTree->size();
		for (size_t i = 0; i < length; ++i)
		{
			// No instance node under the visual scene!
			SceneElement* sceneElement = (*exportNodesTree)[i];

			SceneElement::Type sceneElementType = sceneElement->getType();
			if (sceneElementType == SceneElement::LOD)
			{
				openLibrary();

				// First Pass
				mDocumentExporter->mExportPass = DocumentExporter::PASS::FIRST_LOD_PASS;
				mVisualSceneExporter->exportVisualSceneNodes(sceneElement);

				// Second Pass
				mDocumentExporter->mExportPass = DocumentExporter::PASS::SECOND_LOD_PASS;
				mVisualSceneExporter->exportVisualSceneNodes(sceneElement);
			}
		}

		mDocumentExporter->mExportPass = DocumentExporter::PASS::VISUAL_SCENE_PASS;

		closeLibrary();
    }
}