package synchronizer

import (
	"context"
	"github.com/wso2/product-microgateway/adapter/internal/loggers"
	"github.com/wso2/product-microgateway/adapter/internal/oasparser"
	mgw "github.com/wso2/product-microgateway/adapter/internal/oasparser/model"
	"k8s.io/apimachinery/pkg/types"
)

func HandleAPILifeCycleEvents(store *OperatorDataStore) {
	loggers.LoggerOperator.Info("Operator synchronizer listening for API lifecycle events...")
	ctx := context.Background()
	HandleSubscription(store.APIStore.Subscribe(ctx),
		func(update Update[types.NamespacedName, *APIState]) {
			if update.Delete {
				loggers.LoggerOperator.Infof("DELETE event received: %v", update.Key.String())
			} else {
				loggers.LoggerOperator.Infof("UPDATE event received: %v", update.Key.String())
				deployAPIInGateway(*update.Value)
			}

		},
	)
}

func deployAPIInGateway(apiState APIState) {
	var mgwSwagger mgw.MgwSwagger
	if err := mgwSwagger.SetInfoAPICR(*apiState.APIDefinition); err != nil {
		loggers.LoggerOperator.Errorf("Error setting API CR info to mgwSwagger: %v", err)
	}
	if err := mgwSwagger.SetInfoHttpRouteCR(*apiState.ProdHttpRoute); err != nil {
		loggers.LoggerOperator.Errorf("Error setting HttpRoute CR info to mgwSwagger: %v", err)
	}
	routes, clusters, endpoints, err := oasparser.GetRoutesClustersEndpoints(mgwSwagger, nil, nil, "vhost", "org")
	if err != nil {
		loggers.LoggerOperator.Errorf("Error creating routes and clusters: %v", err)
	}
	loggers.LoggerOperator.Infof("Routes: %v", routes)
	loggers.LoggerOperator.Infof("Clusters: %v", clusters)
	loggers.LoggerOperator.Infof("Endpoints: %v", endpoints)
}
