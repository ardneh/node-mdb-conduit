/**
 * Copyright 2013 (c) Charles J. Ezell III
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


#include "document_source_v8.h"

//The BSON macro doesn't play nicely with the BSON class...
//#undef BSON
//#include <js-bson/ext/bson.h>


using namespace v8;

namespace mongo {

    boost::optional<Document> DocumentSourceV8::getNext() {
		//Always false for now... but I might have a use for it later.
        pExpCtx->checkForInterrupt();

        if (i < length) {

			//Do I have to have a scope or something here??
			Local<v8::Value> element(array->Get(i));

			++i;

			//TODO: use js-bson libaray.  A little more work than I thought it would be, I
			//will have to make my own class based on BSON to get rid of most of the JS assumptions.
			BSONObj obj(converter.v8ToMongo(element->ToObject()));

			return Document(obj);
		} else {
            return boost::none;
		}
    }

    void DocumentSourceV8::setSource(DocumentSource *pSource) {
        /* this doesn't take a source */
        verify(false);
    }

    DocumentSourceV8::DocumentSourceV8(
            const v8::Handle<v8::Array> array,
            const intrusive_ptr<ExpressionContext> &pExpCtx)
        : DocumentSource(pExpCtx)
        , array(array)
        , i(0)
		, length(array->Length())
    {}

    intrusive_ptr<DocumentSourceV8> DocumentSourceV8::create(
            const v8::Handle<v8::Array> array,
            const intrusive_ptr<ExpressionContext> &pExpCtx) {

        return new DocumentSourceV8(array, pExpCtx);
    }

    Value DocumentSourceV8::serialize(bool explain) const {
        if (explain) {
            return Value(DOC("v8Array" << Document()));
        }
        return Value();
    }
}
