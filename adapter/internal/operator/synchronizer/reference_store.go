package synchronizer

import (
	"github.com/orcaman/concurrent-map/v2"
	"k8s.io/apimachinery/pkg/types"
	"sync"
)

type HttpRouteRefs struct {
	ProdHttpRouteRef string
	SandHttpRouteRef string
}

type OperatorReferenceStore struct {
	APIToHttpRouteRefs cmap.ConcurrentMap[HttpRouteRefs]
	HttpRouteToAPIRefs cmap.ConcurrentMap[types.NamespacedName]

	mu sync.Mutex
}

func NewOperatorReferenceStore() *OperatorReferenceStore {
	return &OperatorReferenceStore{
		APIToHttpRouteRefs: cmap.ConcurrentMap[HttpRouteRefs]{},
		HttpRouteToAPIRefs: cmap.ConcurrentMap[types.NamespacedName]{},
	}
}

func (ors *OperatorReferenceStore) GetHttpRouteRefsForAPI(apiName types.NamespacedName) (HttpRouteRefs, bool) {
	return ors.APIToHttpRouteRefs.Get(apiName.String())
}

func (ors *OperatorReferenceStore) UpdateHttpRouteRefsForAPI(apiName types.NamespacedName, refs HttpRouteRefs) error {
	//ors.APIToHttpRouteRefs.Upsert(apiName.String(), refs, func(exists bool, valueInMap HttpRouteRefs, newValue HttpRouteRefs) HttpRouteRefs {
	//	return newValue
	//})
	ors.mu.Lock()
	defer ors.mu.Unlock()

	ors.APIToHttpRouteRefs.Set(apiName.String(), refs)
	ors.HttpRouteToAPIRefs.Set(refs.ProdHttpRouteRef, apiName)
	ors.HttpRouteToAPIRefs.Set(refs.SandHttpRouteRef, apiName)
	return nil
}

func (ors *OperatorReferenceStore) GetAPIRefsForHttpRoute(httpRoute types.NamespacedName) (types.NamespacedName, bool) {
	return ors.HttpRouteToAPIRefs.Get(httpRoute.String())
}
