package synchronizer

import (
	"fmt"

	"github.com/telepresenceio/watchable"
	dpv1alpha1 "github.com/wso2/product-microgateway/adapter/internal/operator/api/v1alpha1"
	"github.com/wso2/product-microgateway/adapter/internal/operator/utils"
	"k8s.io/apimachinery/pkg/types"
	gwapiv1b1 "sigs.k8s.io/gateway-api/apis/v1beta1"
	"sync"
)

type OperatorDataStore struct {
	APIStore           watchable.Map[types.NamespacedName, *APIState]
	APIToHttpRouteRefs map[types.NamespacedName]HttpRouteRefs
	HttpRouteToAPIRefs map[types.NamespacedName]types.NamespacedName

	mu sync.Mutex
}

type HttpRouteRefs struct {
	ProdHttpRouteRef string
	SandHttpRouteRef string
}

func CreateNewOperatorDataStore() *OperatorDataStore {
	return &OperatorDataStore{
		APIStore:           watchable.Map[types.NamespacedName, *APIState]{},
		APIToHttpRouteRefs: map[types.NamespacedName]HttpRouteRefs{},
		HttpRouteToAPIRefs: map[types.NamespacedName]types.NamespacedName{},
	}
}

func (ods *OperatorDataStore) AddNewAPI(api dpv1alpha1.API, prodHttpRoute gwapiv1b1.HTTPRoute, sandHttpRoute gwapiv1b1.HTTPRoute) error {
	ods.mu.Lock()
	defer ods.mu.Unlock()

	ods.APIStore.Store(utils.NamespacedName(&api),
		&APIState{
			APIDefinition: &api,
			ProdHttpRoute: &prodHttpRoute,
			SandHttpRoute: &sandHttpRoute,
		})
	//ods.APIToHttpRouteRefs.Set(utils.NamespacedName(&api).String(), HttpRouteRefs{
	//	ProdHttpRouteRef: prodHttpRoute.Name,
	//	SandHttpRouteRef: "foo",
	//})
	ods.APIToHttpRouteRefs[utils.NamespacedName(&api)] = HttpRouteRefs{ProdHttpRouteRef: prodHttpRoute.Name, SandHttpRouteRef: ""}
	ods.HttpRouteToAPIRefs[utils.NamespacedName(&prodHttpRoute)] = utils.NamespacedName(&api)
	ods.HttpRouteToAPIRefs[utils.NamespacedName(&sandHttpRoute)] = utils.NamespacedName(&api)
	return nil
}

func (ods *OperatorDataStore) UpdateHttpRoute(apiName types.NamespacedName, httpRoute gwapiv1b1.HTTPRoute, production bool) error {
	ods.mu.Lock()
	defer ods.mu.Unlock()

	apiState, found := ods.APIStore.Load(apiName)
	if !found {
		return fmt.Errorf("Error: No API found for the key: %v", apiName.String())
	}

	if production {
		apiState.ProdHttpRoute = &httpRoute
	} else {
		apiState.SandHttpRoute = &httpRoute
	}
	return nil
}

func (ods *OperatorDataStore) UpdateAPIDef(apiDef dpv1alpha1.API) error {
	api, found := ods.APIStore.Load(utils.NamespacedName(&apiDef))
	if !found {
		return fmt.Errorf("API not found in the Operator Data store: %v", apiDef.Spec.APIDisplayName)
	}
	api.APIDefinition = &apiDef
	return nil
}

func (ods *OperatorDataStore) GetAPI(apiName types.NamespacedName) (APIState, bool) {
	_, found := ods.APIStore.Load(apiName)
	if !found {
		return APIState{}, found
	} else {
		return APIState{}, found
	}
}

//func (ods *OperatorDataStore) GetHttpRouteRefsForAPI(apiName types.NamespacedName) (HttpRouteRefs, bool) {
//	return ods.APIToHttpRouteRefs.Get(apiName.String())
//}
//
//func (ods *OperatorDataStore) UpdateHttpRouteRefsForAPI(apiName types.NamespacedName, refs HttpRouteRefs) error {
//	//ors.APIToHttpRouteRefs.Upsert(apiName.String(), refs, func(exists bool, valueInMap HttpRouteRefs, newValue HttpRouteRefs) HttpRouteRefs {
//	//	return newValue
//	//})
//	ods.mu.Lock()
//	defer ods.mu.Unlock()
//
//	ods.APIToHttpRouteRefs.Set(apiName.String(), refs)
//	ods.HttpRouteToAPIRefs.Set(refs.ProdHttpRouteRef, apiName)
//	ods.HttpRouteToAPIRefs.Set(refs.SandHttpRouteRef, apiName)
//	return nil
//}
//
//func (ods *OperatorDataStore) GetAPIRefsForHttpRoute(httpRoute types.NamespacedName) (types.NamespacedName, bool) {
//	return ods.HttpRouteToAPIRefs.Get(httpRoute.String())
//}
