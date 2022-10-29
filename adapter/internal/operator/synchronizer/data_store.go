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
	APIStore watchable.Map[types.NamespacedName, *APIState]

	mu sync.Mutex
}

func CreateNewOperatorDataStore() *OperatorDataStore {
	return &OperatorDataStore{
		APIStore: watchable.Map[types.NamespacedName, *APIState]{},
	}
}

func (ods *OperatorDataStore) AddNewAPI(api dpv1alpha1.API, prodHttpRoute gwapiv1b1.HTTPRoute, sandHttpRoute gwapiv1b1.HTTPRoute) error {
	ods.APIStore.Store(utils.NamespacedName(&api),
		&APIState{
			APIDefinition: &api,
			ProdHttpRoute: &prodHttpRoute,
			SandHttpRoute: &sandHttpRoute,
		})
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
