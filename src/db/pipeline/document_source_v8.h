/**
 * Copyright 2011 (c) 10gen Inc.
 *
 * This program is free software: you can redistribute it and/or  modify
 * it under the terms of the GNU Affero General Public License, version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, the copyright holders give permission to link the
 * code of portions of this program with the OpenSSL library under certain
 * conditions as described in each individual source file and distribute
 * linked combinations including the program with the OpenSSL library. You
 * must comply with the GNU Affero General Public License in all respects for
 * all of the code used other than as permitted herein. If you modify file(s)
 * with this exception, you may extend this exception to your version of the
 * file(s), but you are not obligated to do so. If you do not wish to do so,
 * delete this exception statement from your version. If you delete this
 * exception statement from all source files in the program, then also delete
 * it in the license file.
 */


#pragma once


#include <v8.h>

#include "mongo/db/pipeline/document_source.h"

//TODO: Get rid of this ASAP.
#include "MongoV8Helpers.h"


namespace mongo {

    class DocumentSourceV8 :
        public DocumentSource {
    public:
        // virtuals from DocumentSource
        virtual boost::optional<Document> getNext();
        virtual Value serialize(bool explain = false) const;
        virtual void setSource(DocumentSource *pSource);
        virtual bool isValidInitialSource() const { return true; }

        /**
          Create a document source based on a v8 array.

          @param array the v8 array to treat as a document source
          @param pExpCtx the expression context for the pipeline
          @returns the newly created document source
        */
        static intrusive_ptr<DocumentSourceV8> create(
            const v8::Handle<v8::Array> array,
            const intrusive_ptr<ExpressionContext> &pExpCtx);

    private:
        DocumentSourceV8(
            const v8::Handle<v8::Array> array,
            const intrusive_ptr<ExpressionContext> &pExpCtx);

		v8::Handle<v8::Array> array;
        uint32_t i;
		const uint32_t length;

		not_mongo::MongoV8Helpers converter;
    };

}
