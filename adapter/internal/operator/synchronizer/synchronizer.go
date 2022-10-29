package synchronizer

import (
	"context"
	"github.com/wso2/product-microgateway/adapter/internal/loggers"
	"k8s.io/apimachinery/pkg/types"
)

func HandleAPILifeCycleEvents(store *OperatorDataStore) {
	loggers.LoggerOperator.Info("Operator synchronizer listening for API lifecycle events...")
	ctx := context.Background()
	HandleSubscription(store.APIStore.Subscribe(ctx),
		func(update Update[types.NamespacedName, *APIState]) {
			if update.Delete {
				loggers.LoggerOperator.Infof("DELETE event received: %v", update.Key.String())
			}
			loggers.LoggerOperator.Infof("UPDATE event received: %v", update.Key.String())
		},
	)
}
