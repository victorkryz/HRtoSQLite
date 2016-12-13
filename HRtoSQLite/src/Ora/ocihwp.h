/**
 * HRtoSQLite
 *
 * @author Victor Kryzhanivskyi
 */

#ifndef _ocihwp_h_
#define _ocihwp_h_

#include "oci.h"
#include <assert.h>
#include <iostream>
#include <stdexcept>

namespace ocihwp
{
	namespace hlftm
	{
		template <typename T, const int id> class handleId
		{
		public:
			static int getId() {
				return id;
			}
		};
		template <typename T> ub4 getHandleId(T*);
		
		template <typename T> sword free(T* pHnd)
		{
			sword swStatus(OCI_SUCCESS);
			if (nullptr != pHnd)
			{
				swStatus = OCIHandleFree(pHnd, getHandleId((T*)(nullptr)));
				assert(OCI_SUCCESS == swStatus);
			}

			return swStatus;
		}

		template<> inline sword free(OCIDefine* pHnd) {
			return OCI_SUCCESS;
		}
	}

	template <typename T> class OciHandle
	{
	public:
		OciHandle() {}
		OciHandle(T* pHnd, bool bOwner = true) :
			pHnd_(pHnd), bOwner_(bOwner) {
		}
		OciHandle(OciHandle&& hndFrom)
		{
			pHnd_ = hndFrom.pHnd_;
			bOwner_ = hndFrom.bOwner_;

			hndFrom.pHnd_ = nullptr;
			hndFrom.bOwner_ = false;
		}

		~OciHandle() {
			if (bOwner_ && (nullptr != pHnd_))
				hlftm::free(pHnd_);
		}

		T* get() { return pHnd_; };
		T*& getRef() { return pHnd_; };

		T** operator&() { return &pHnd_; }
		T* operator*() { return pHnd_; }

	protected:
		T* pHnd_ = nullptr;
		bool bOwner_ = true;
	};

	
	template <typename T, typename E = OCIEnv> class HandleFactory
	{
		public:
			static T* allocate(const E* pEnv)
			{
				T* pHnd = nullptr;

				auto id = hlftm::getHandleId(static_cast<T*>(nullptr));
				const auto stat = OCIHandleAlloc(pEnv, reinterpret_cast<void**>(&pHnd), id, 0, nullptr);
				assert(stat == OCI_SUCCESS);
				if (stat != OCI_SUCCESS)
					throw std::runtime_error("OCIHandleAlloc() failed");

				return pHnd;
			}
	};

	namespace hlftm
	{
		template<> inline ub4 getHandleId(OCIEnv*)   {return OCI_HTYPE_ENV;}
		template<> inline ub4 getHandleId(OCIError*) {return OCI_HTYPE_ERROR;}
		template<> inline ub4 getHandleId(OCISvcCtx*) { return OCI_HTYPE_SVCCTX;}
		template<> inline ub4 getHandleId(OCIServer*) { return OCI_HTYPE_SERVER;}
		template<> inline ub4 getHandleId(OCIStmt*) { return OCI_HTYPE_STMT;}
		template<> inline ub4 getHandleId(OCISession*) { return OCI_HTYPE_SESSION; }
		template<> inline ub4 getHandleId(OCIDefine*) { return OCI_HTYPE_DEFINE; }
		template<> inline ub4 getHandleId(OCIDescribe*) { return OCI_HTYPE_DESCRIBE; }
	}

} //namespace ocihwp


#endif  // _ocihwp_h_